#include "BaseAICharacter.h"
#include "QuestTargetComponent.h"
#include "QuestGiverComponent.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "ShooterWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

ABaseAICharacter::ABaseAICharacter()
{
	// Create components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	QuestTarget = CreateDefaultSubobject<UQuestTargetComponent>(TEXT("QuestTarget"));
	// QuestGiver component is created in derived classes (e.g., QuestGiverNPC) that need it
	QuestGiver = nullptr;

	// Create interaction sphere for quest/interaction detection
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(InteractionRadius);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionSphere->SetGenerateOverlapEvents(true);

	// Create text render component for interaction prompt
	PromptText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PromptText"));
	PromptText->SetupAttachment(RootComponent);
	PromptText->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f)); // Above NPC head
	PromptText->SetWorldSize(45.0f);
	PromptText->SetTextRenderColor(FColor::White);
	PromptText->SetHorizontalAlignment(EHTA_Center);
	PromptText->SetVerticalAlignment(EVRTA_TextCenter);
	PromptText->SetVisibility(false); // Hidden by default
	
	// Make text always face camera
	PromptText->SetHiddenInGame(false);
	PromptText->bAlwaysRenderAsText = true;

	// Disable tick by default
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get QuestGiver component (might be added in Blueprint)
	if (!QuestGiver)
	{
		QuestGiver = FindComponentByClass<UQuestGiverComponent>();
	}
	
	if (QuestGiver)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> NPC INIT: [%s] QuestGiver component found: %p"), 
			*GetName(), QuestGiver);
	}

	// Update interaction sphere radius
	if (InteractionSphere)
	{
		InteractionSphere->SetSphereRadius(InteractionRadius);
		UE_LOG(LogTemp, Display, TEXT(">>> NPC INIT: [%s] InteractionSphere radius set to %f cm"), 
			*GetName(), InteractionRadius);
	}

	// Enable tick for text rotation
	PrimaryActorTick.bCanEverTick = true;

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

void ABaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make prompt text face camera
	if (PromptText && PromptText->IsVisible())
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
			FVector TextLocation = PromptText->GetComponentLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TextLocation, CameraLocation);
			PromptText->SetWorldRotation(LookAtRotation);
		}
	}

	// Debug visualization for interaction sphere
	if (bShowInteractionSphere && InteractionSphere)
	{
		FVector Location = GetActorLocation();
		
		// Draw persistent cyan sphere
		DrawDebugSphere(GetWorld(), Location, InteractionRadius, 24, FColor::Cyan, false, 0.0f, 0, 3.0f);
		
		// Also draw the actual sphere component bounds for comparison
		FVector SphereLocation = InteractionSphere->GetComponentLocation();
		float SphereRadius = InteractionSphere->GetScaledSphereRadius();
		DrawDebugSphere(GetWorld(), SphereLocation, SphereRadius, 16, FColor::Green, false, 0.0f, 0, 2.0f);
	}
}

// === IQuestInteractable INTERFACE ===

bool ABaseAICharacter::CanInteract_Implementation(AActor* Interactor) const
{
	// Can interact if alive and has quest giver component
	return !IsDead() && QuestGiver != nullptr;
}

FText ABaseAICharacter::GetInteractionText_Implementation() const
{
	if (QuestGiver)
	{
		return FText::FromString(TEXT("Talk"));
	}
	return FText::FromString(TEXT("Interact"));
}

void ABaseAICharacter::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Display, TEXT(">>> NPC INTERACT: [%s] interacted with by [%s]"), 
		*GetName(), *Interactor->GetName());

	// Quest giver component handles both turn-in and quest offering
	if (!QuestGiver)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> NPC INTERACT: No QuestGiverComponent found on [%s]"), *GetName());
		return;
	}

	if (!Interactor)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> NPC INTERACT: Interactor is null!"));
		return;
	}

	// HandleInteraction automatically checks for completed quests first, then offers new quests
	QuestGiver->HandleInteraction(Interactor);
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

void ABaseAICharacter::ShowPrompt_Implementation()
{
	if (PromptText)
	{
		// Show "Press E to talk" for quest givers, or generic interaction text
		FText PromptMessage = QuestGiver ? 
			FText::FromString(TEXT("Press E to talk")) : 
			FText::FromString(TEXT("Press E to interact"));
		
		PromptText->SetText(PromptMessage);
		PromptText->SetVisibility(true);
		UE_LOG(LogTemp, Display, TEXT(">>> NPC PROMPT: Showing prompt on [%s]: %s"), 
			*GetName(), *PromptMessage.ToString());
	}
}

void ABaseAICharacter::HidePrompt_Implementation()
{
	if (PromptText)
	{
		PromptText->SetVisibility(false);
		UE_LOG(LogTemp, Display, TEXT(">>> NPC PROMPT: Hiding prompt on [%s]"), *GetName());
	}
}
