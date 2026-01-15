#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "CombatSystem/Interfaces/CombatInterface.h"
#include "TimerManager.h"
#include "Engine/World.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultComboData)
	{
		ActiveComboData = DefaultComboData;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::StartAttack(EAttackType AttackType)
{
	UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: StartAttack called - Type: %d, ComboIndex: %d"), (int32)AttackType, CurrentComboIndex);
	
	if (!CanAttack())
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> COMBAT: CanAttack returned false - State: %d, ComboWindowOpen: %d"), (int32)CurrentCombatState, bComboWindowOpen);
		if (bComboWindowOpen && bCanCombo)
		{
			bAttackQueued = true;
			QueuedAttackType = AttackType;
			UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Attack queued"));
		}
		return;
	}

	if (!ActiveComboData || !ActiveComboData->IsValidComboIndex(CurrentComboIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::StartAttack - No valid combo data"));
		return;
	}

	FComboAttack CurrentAttack = ActiveComboData->GetComboAttack(CurrentComboIndex);

	if (CurrentAttack.AttackMontage)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Playing montage: %s"), *CurrentAttack.AttackMontage->GetName());
		PlayAttackMontage(CurrentAttack);
		SetCombatState(ECombatState::Attacking);
		HitActorsThisAttack.Empty();

		OnAttackStarted.Broadcast(AttackType);

		if (bAutoResetCombo)
		{
			GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> COMBAT: No montage set for combo index %d"), CurrentComboIndex);
	}
}

void UCombatComponent::StopAttack()
{
	SetCombatState(ECombatState::Idle);
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->StopAllMontages(0.2f);
		}
	}

	OnAttackEnded.Broadcast();
}

void UCombatComponent::PerformMeleeTrace(const FMeleeCollisionInfo& CollisionInfo)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->GetMesh())
	{
		return;
	}

	USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh();
	FVector SocketLocation = MeshComp->GetSocketLocation(CollisionInfo.SocketName);

	if (SocketLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::PerformMeleeTrace - Invalid socket: %s"), *CollisionInfo.SocketName.ToString());
		return;
	}

	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(CollisionInfo.TraceRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SocketLocation,
		SocketLocation,
		FQuat::Identity,
		DamageTraceChannel,
		SphereShape,
		QueryParams
	);

	if (CollisionInfo.bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), SocketLocation, CollisionInfo.TraceRadius, 12, 
			bHit ? FColor::Green : FColor::Red, false, 0.5f);
	}

	if (bHit && ActiveComboData && ActiveComboData->IsValidComboIndex(CurrentComboIndex))
	{
		FComboAttack CurrentAttack = ActiveComboData->GetComboAttack(CurrentComboIndex);
		float FinalDamage = CurrentAttack.Damage * BaseDamageMultiplier;

		for (const FHitResult& Hit : HitResults)
		{
			if (!HasAlreadyHitActor(Hit.GetActor()))
			{
				ProcessHit(Hit, FinalDamage);
			}
		}
	}
}

void UCombatComponent::ResetCombo()
{
	CurrentComboIndex = 0;
	bComboWindowOpen = false;
	bAttackQueued = false;
	HitActorsThisAttack.Empty();

	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ComboWindowTimerHandle);

	OnComboReset.Broadcast();
}

bool UCombatComponent::CanAttack() const
{
	return CurrentCombatState == ECombatState::Idle || 
	       (CurrentCombatState == ECombatState::Attacking && bComboWindowOpen);
}

void UCombatComponent::SetCombatState(ECombatState NewState)
{
	if (CurrentCombatState != NewState)
	{
		CurrentCombatState = NewState;
		OnCombatStateChanged.Broadcast(NewState);
	}
}

void UCombatComponent::RegisterHit(AActor* HitActor, const FHitData& HitData)
{
	if (HitActor && !HasAlreadyHitActor(HitActor))
	{
		HitActorsThisAttack.Add(HitActor);
		OnHitLanded.Broadcast(HitActor, HitData);
	}
}

void UCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetCombatState(ECombatState::Idle);
	OnAttackEnded.Broadcast();

	if (bAutoResetCombo && ActiveComboData)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ComboResetTimerHandle,
			this,
			&UCombatComponent::ResetCombo,
			ActiveComboData->ComboResetTime,
			false
		);
	}
}

void UCombatComponent::OnComboWindowOpened()
{
	UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Combo window OPENED"));
	bComboWindowOpen = true;

	if (bAttackQueued)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Processing queued attack"));
		bAttackQueued = false;
		ProgressCombo();
		StartAttack(QueuedAttackType);
	}
}

void UCombatComponent::OnComboWindowClosed()
{
	UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Combo window CLOSED"));
	bComboWindowOpen = false;
	bAttackQueued = false;
}

void UCombatComponent::PlayAttackMontage(const FComboAttack& Attack)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->GetMesh())
	{
		return;
	}

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance || !Attack.AttackMontage)
	{
		return;
	}

	AnimInstance->Montage_Play(Attack.AttackMontage);

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &UCombatComponent::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Attack.AttackMontage);

	if (Attack.ComboWindowStart > 0.0f)
	{
		FTimerDelegate ComboWindowOpenDelegate;
		ComboWindowOpenDelegate.BindUObject(this, &UCombatComponent::OnComboWindowOpened);
		GetWorld()->GetTimerManager().SetTimer(
			ComboWindowTimerHandle,
			ComboWindowOpenDelegate,
			Attack.ComboWindowStart,
			false
		);
	}

	if (Attack.ComboWindowEnd > Attack.ComboWindowStart)
	{
		FTimerDelegate ComboWindowCloseDelegate;
		ComboWindowCloseDelegate.BindUObject(this, &UCombatComponent::OnComboWindowClosed);
		GetWorld()->GetTimerManager().SetTimer(
			ComboWindowTimerHandle,
			ComboWindowCloseDelegate,
			Attack.ComboWindowEnd,
			false
		);
	}
}

void UCombatComponent::ProgressCombo()
{
	if (!ActiveComboData)
	{
		return;
	}

	int32 OldIndex = CurrentComboIndex;
	CurrentComboIndex++;

	if (CurrentComboIndex >= ActiveComboData->GetComboLength())
	{
		if (ActiveComboData->bLoopCombo)
		{
			CurrentComboIndex = 0;
			UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Combo looped from %d to 0"), OldIndex);
		}
		else
		{
			CurrentComboIndex = ActiveComboData->GetComboLength() - 1;
			UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Combo clamped at max index %d"), CurrentComboIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> COMBAT: Combo progressed from %d to %d"), OldIndex, CurrentComboIndex);
	}

	OnComboProgressed.Broadcast(CurrentComboIndex);
	HitActorsThisAttack.Empty();
}

void UCombatComponent::ProcessHit(const FHitResult& HitResult, float Damage)
{
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return;
	}

	FHitData HitData;
	HitData.HitActor = HitActor;
	HitData.HitLocation = HitResult.ImpactPoint;
	HitData.HitNormal = HitResult.ImpactNormal;
	HitData.Damage = Damage;
	HitData.HitReaction = EHitReaction::Light;

	if (HitActor->Implements<UCombatInterface>())
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor);
		if (CombatInterface && CombatInterface->Execute_CanBeDamaged(HitActor))
		{
			CombatInterface->Execute_TakeCombatDamage(HitActor, HitData, GetOwner());
			RegisterHit(HitActor, HitData);
		}
	}
}

bool UCombatComponent::HasAlreadyHitActor(AActor* Actor) const
{
	return HitActorsThisAttack.Contains(Actor);
}
