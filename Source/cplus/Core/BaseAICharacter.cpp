#include "BaseAICharacter.h"
#include "QuestTargetComponent.h"
#include "QuestGiverComponent.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "ShooterWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

ABaseAICharacter::ABaseAICharacter()
{
	// Create components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	QuestTarget = CreateDefaultSubobject<UQuestTargetComponent>(TEXT("QuestTarget"));
	QuestGiver = CreateDefaultSubobject<UQuestGiverComponent>(TEXT("QuestGiver"));

	// Disable tick by default
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind to health component death event
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABaseAICharacter::OnHealthDepleted);
	}
}

void ABaseAICharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear timers
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

// === IQuestInteractable INTERFACE ===

void ABaseAICharacter::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("BaseAICharacter: %s interacted with by %s"), 
		*GetName(), *Interactor->GetName());

	// Quest giver component handles quest offering
	if (QuestGiver)
	{
		// QuestGiver->OfferQuests(Interactor);
	}
}

// === IQuestKillable INTERFACE ===

void ABaseAICharacter::OnKilledForQuest_Implementation(AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("BaseAICharacter: %s killed for quest by %s"), 
		*GetName(), *Killer->GetName());

	// Notify quest system via QuestTargetComponent
	if (QuestTarget)
	{
		// QuestTarget->NotifyKilled(Killer);
	}
}

// === IShooterWeaponHolder INTERFACE ===

void ABaseAICharacter::AttachWeaponMeshes(AShooterWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// Attach the weapon actor
	Weapon->AttachToActor(this, AttachmentRule);

	// AI only uses third person mesh
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, ThirdPersonWeaponSocket);
}

void ABaseAICharacter::PlayFiringMontage(UAnimMontage* Montage)
{
	// Unused for AI
}

void ABaseAICharacter::AddWeaponRecoil(float Recoil)
{
	// Unused for AI
}

void ABaseAICharacter::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	// Unused for AI
}

FVector ABaseAICharacter::GetWeaponTargetLocation()
{
	// AI uses actor eye location for aiming
	const FVector AimSource = GetActorLocation() + FVector(0, 0, BaseEyeHeight);

	FVector AimDir, AimTarget = FVector::ZeroVector;

	// Do we have an aim target?
	if (CurrentAimTarget)
	{
		// Target the actor location
		AimTarget = CurrentAimTarget->GetActorLocation();

		// Apply a vertical offset to target head/feet
		AimTarget.Z += FMath::RandRange(MinAimOffsetZ, MaxAimOffsetZ);

		// Get the aim direction and apply randomness in a cone
		AimDir = (AimTarget - AimSource).GetSafeNormal();
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(AimDir, AimVarianceHalfAngle);
	}
	else
	{
		// No aim target, so just use the actor forward
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetActorForwardVector(), AimVarianceHalfAngle);
	}

	// Calculate the unobstructed aim target location
	AimTarget = AimSource + (AimDir * AimRange);

	// Run a visibility trace to see if there's obstructions
	FHitResult OutHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, AimSource, AimTarget, ECC_Visibility, QueryParams);

	// Return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void ABaseAICharacter::AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	if (WeaponComponent)
	{
		WeaponComponent->AddWeaponClass(WeaponClass);
	}
}

void ABaseAICharacter::OnWeaponActivated(AShooterWeapon* Weapon)
{
	// Unused for AI
}

void ABaseAICharacter::OnWeaponDeactivated(AShooterWeapon* Weapon)
{
	// Unused for AI
}

void ABaseAICharacter::OnSemiWeaponRefire()
{
	// Are we still shooting?
	if (bIsShooting && WeaponComponent)
	{
		// Fire the weapon
		WeaponComponent->StartFiring();
	}
}

// === SHOOTING API ===

void ABaseAICharacter::StartShooting(AActor* ActorToShoot)
{
	// Save the aim target
	CurrentAimTarget = ActorToShoot;

	// Raise the flag
	bIsShooting = true;

	// Signal the weapon
	if (WeaponComponent)
	{
		WeaponComponent->StartFiring();
	}
}

void ABaseAICharacter::StopShooting()
{
	// Lower the flag
	bIsShooting = false;

	// Signal the weapon
	if (WeaponComponent)
	{
		WeaponComponent->StopFiring();
	}
}

// === GETTERS ===

bool ABaseAICharacter::IsDead() const
{
	return HealthComponent ? HealthComponent->IsDead() : false;
}

float ABaseAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead())
	{
		return 0.0f;
	}

	// Route damage to health component
	if (HealthComponent)
	{
		return HealthComponent->ApplyDamage(Damage, DamageCauser);
	}

	return 0.0f;
}

// === DEATH/RAGDOLL ===

void ABaseAICharacter::OnHealthDepleted(AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("BaseAICharacter: %s died"), *GetName());

	// Notify quest system
	OnKilledForQuest_Implementation(Killer);

	// Grant the death tag to the character
	Tags.Add(DeathTag);

	// Call the delegate
	OnAIPawnDeath.Broadcast();

	// Disable capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Stop movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();

	// Enable ragdoll physics on the third person mesh
	GetMesh()->SetCollisionProfileName(RagdollCollisionProfile);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsBlendWeight(1.0f);

	// Schedule actor destruction
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ABaseAICharacter::DeferredDestruction, DeferredDestructionTime, false);
}

void ABaseAICharacter::DeferredDestruction()
{
	Destroy();
}
