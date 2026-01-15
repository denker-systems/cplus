#include "BasePlayerCharacter.h"
#include "QuestTrackerComponent.h"
#include "InventoryComponent.h"
#include "InteractionComponent.h"
#include "HealthComponent.h"
#include "WeaponSystem/Components/WeaponComponent.h"
#include "PlayerProgressionComponent.h"
#include "UI/UIManager.h"
#include "QuestSystem/UI/QuestUIManager.h"
#include "WeaponSystem/Actors/BaseWeapon.h"
#include "QuestInteractable.h"
#include "QuestDefinition.h"
#include "QuestGiverComponent.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Constructor called"));
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure the character mesh for third-person
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));
	GetMesh()->SetOwnerNoSee(false);

	// Create SpringArm for third-person camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create the spotlight for horror mode
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(FollowCamera);
	SpotLight->SetRelativeLocationAndRotation(FVector(30.0f, 17.5f, -5.0f), FRotator(-18.6f, -1.3f, 5.26f));
	SpotLight->Intensity = 0.5;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 1050.0f;
	SpotLight->InnerConeAngle = 18.7f;
	SpotLight->OuterConeAngle = 45.24f;
	SpotLight->SetVisibility(false); // Disabled by default

	// Create the noise emitter component for shooter mode
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));

	// Create player-specific components
	QuestTracker = CreateDefaultSubobject<UQuestTrackerComponent>(TEXT("QuestTracker"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	// HealthComponent - Add manually in Blueprint (Details panel issue with native component)
	// UIManager - Add manually in Blueprint (Details panel issue with native component)
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	ProgressionComponent = CreateDefaultSubobject<UPlayerProgressionComponent>(TEXT("ProgressionComponent"));

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Disable tick by default
	PrimaryActorTick.bCanEverTick = false;
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: BeginPlay called for %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Location = %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Camera = %s"), FollowCamera ? TEXT("Valid") : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Mesh = %s"), GetMesh() ? TEXT("Valid") : TEXT("NULL"));

	// Find UIManager component (added manually in Blueprint)
	if (!UIManager)
	{
		UIManager = FindComponentByClass<UUIManager>();
		if (UIManager)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: Found UIManager component"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> UI MANAGER: UIManager component not found! Add it in Blueprint"));
		}
	}

	// Bind to health component death event
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABasePlayerCharacter::OnHealthDepleted);
	}

	// Initialize sprint meter to max
	SprintMeter = SprintTime;

	// Start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &ABasePlayerCharacter::SprintFixedTick, SprintFixedTickTime, true);
}

void ABasePlayerCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear timers
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}

void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: SetupPlayerInputComponent called"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: JumpAction = %s"), JumpAction ? *JumpAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: MoveAction = %s"), MoveAction ? *MoveAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: LookAction = %s"), LookAction ? *LookAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: MouseLookAction = %s"), MouseLookAction ? *MouseLookAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: FireAction = %s"), FireAction ? *FireAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: SwitchWeaponAction = %s"), SwitchWeaponAction ? *SwitchWeaponAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: SprintAction = %s"), SprintAction ? *SprintAction->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: CrouchAction = %s"), CrouchAction ? *CrouchAction->GetName() : TEXT("NULL"));

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::DoJumpStart);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::DoJumpEnd);
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Jump input bound"));
		}

		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::MoveInput);
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Move input bound"));
		}

		// Looking/Aiming
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::LookInput);
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Look input bound (keyboard)"));
		}
		
		// Mouse Look (from IMC_MouseLook)
		if (MouseLookAction)
		{
			EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::LookInput);
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: MouseLook input bound (mouse)"));
		}

		// Firing
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoStartFiring);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::DoStopFiring);
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Fire input bound (Triggered)"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BasePlayerCharacter: FireAction is NULL!"));
		}

		// Switch weapon
		if (SwitchWeaponAction)
		{
			EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoSwitchWeapon);
		}

		// Reload
		if (ReloadAction)
		{
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoReload);
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Reload input bound"));
		}

		// Sprinting
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::DoEndSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ABasePlayerCharacter::DoEndSprint);
		}

		// Crouching
		if (CrouchAction)
		{
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::DoStartCrouch);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoStartCrouch);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::DoEndCrouch);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Canceled, this, &ABasePlayerCharacter::DoEndCrouch);
		}

		// Interaction
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoInteract);
		}

		// Open Quest Journal
		if (OpenJournalAction)
		{
			EnhancedInputComponent->BindAction(OpenJournalAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoOpenJournal);
		}

		// Pause Menu
		if (PauseAction)
		{
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoPauseMenu);
		}

		// Inventory
		if (InventoryAction)
		{
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoToggleInventory);
		}
	}
}

// === INPUT HANDLERS ===

void ABasePlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ABasePlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void ABasePlayerCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// Pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ABasePlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController() && !IsDead())
	{
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> INPUT: DoMove blocked - Controller=%s, IsDead=%s"),
			GetController() ? TEXT("Valid") : TEXT("NULL"),
			IsDead() ? TEXT("true") : TEXT("false"));
	}
}

void ABasePlayerCharacter::DoJumpStart()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoJumpStart called"));
	
	if (!IsDead())
	{
		Jump();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> INPUT: Jump blocked - IsDead=true"));
	}
}

void ABasePlayerCharacter::DoJumpEnd()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoJumpEnd called"));
	
	if (!IsDead())
	{
		StopJumping();
	}
}

void ABasePlayerCharacter::DoStartFiring()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoStartFiring called"));
	
	if (WeaponComponent && !IsDead())
	{
		WeaponComponent->StartFiring();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> INPUT: Fire blocked - WeaponComponent=%s, IsDead=%s"),
			WeaponComponent ? TEXT("Valid") : TEXT("NULL"),
			IsDead() ? TEXT("true") : TEXT("false"));
	}
}

void ABasePlayerCharacter::DoStopFiring()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoStopFiring called"));
	
	if (WeaponComponent && !IsDead())
	{
		WeaponComponent->StopFiring();
	}
}

void ABasePlayerCharacter::DoSwitchWeapon()
{
	if (WeaponComponent && !IsDead())
	{
		WeaponComponent->SwitchWeapon();
	}
}

void ABasePlayerCharacter::DoReload()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoReload called"));
	
	if (WeaponComponent && !IsDead())
	{
		WeaponComponent->StartReload();
		UE_LOG(LogTemp, Display, TEXT(">>> INPUT: StartReload executed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> INPUT: Reload blocked - WeaponComponent=%s, IsDead=%s"),
			WeaponComponent ? TEXT("Valid") : TEXT("NULL"),
			IsDead() ? TEXT("true") : TEXT("false"));
	}
}

void ABasePlayerCharacter::DoStartSprint()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoStartSprint called"));
	
	bSprinting = true;

	if (!bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		OnSprintStateChanged.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> INPUT: Sprint blocked - bRecovering=true"));
	}
}

void ABasePlayerCharacter::DoEndSprint()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoEndSprint called"));
	
	bSprinting = false;

	if (!bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		OnSprintStateChanged.Broadcast(false);
	}
}

void ABasePlayerCharacter::DoStartCrouch()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoStartCrouch called"));

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	if (!MovementComponent->GetNavAgentPropertiesRef().bCanCrouch)
	{
		return;
	}

	Crouch();
}

void ABasePlayerCharacter::DoEndCrouch()
{
	UE_LOG(LogTemp, Display, TEXT(">>> INPUT: DoEndCrouch called"));
	UnCrouch();
}

void ABasePlayerCharacter::SprintFixedTick()
{
	// Are we out of recovery, still have stamina and are moving faster than our walk speed?
	if (bSprinting && !bRecovering && GetVelocity().Length() > WalkSpeed)
	{
		// Do we still have meter to burn?
		if (SprintMeter > 0.0f)
		{
			// Update the sprint meter
			SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);

			// Have we run out of stamina?
			if (SprintMeter <= 0.0f)
			{
				// Raise the recovering flag
				bRecovering = true;

				// Set the recovering walk speed
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}
	}
	else
	{
		// Recover stamina
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (SprintMeter >= SprintTime)
		{
			// Lower the recovering flag
			bRecovering = false;

			// Set the walk or sprint speed depending on whether the sprint button is down
			GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;

			// Update the sprint state depending on whether the button is down or not
			OnSprintStateChanged.Broadcast(bSprinting);
		}
	}

	// Broadcast the sprint meter updated delegate
	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}

// === DEATH/RESPAWN ===

void ABasePlayerCharacter::OnHealthDepleted(AActor* Killer)
{
	// Deactivate the weapon
	if (WeaponComponent)
	{
		WeaponComponent->DeactivateCurrentWeapon();
	}

	// Grant the death tag to the character
	Tags.Add(DeathTag);

	// Stop character movement
	GetCharacterMovement()->StopMovementImmediately();

	// Disable controls
	DisableInput(nullptr);

	// Call the BP handler
	BP_OnDeath();

	// Schedule character respawn
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ABasePlayerCharacter::OnRespawn, RespawnTime, false);
}

void ABasePlayerCharacter::OnRespawn()
{
	// Destroy the character to force the PC to respawn
	Destroy();
}

// === IWeaponHolder INTERFACE ===

void ABasePlayerCharacter::AttachWeaponMeshes(ABaseWeapon* Weapon)
{
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON ATTACH: Weapon is null!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> WEAPON ATTACH: Attaching %s to socket %s"), *Weapon->GetName(), *ThirdPersonWeaponSocket.ToString());

	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// Attach the weapon actor
	Weapon->AttachToActor(this, AttachmentRule);

	// Attach the weapon mesh (third-person only)
	USkeletalMeshComponent* WeaponMesh = Weapon->GetThirdPersonMesh();
	if (WeaponMesh)
	{
		WeaponMesh->AttachToComponent(GetMesh(), AttachmentRule, ThirdPersonWeaponSocket);
		
		// Debug mesh info
		USkeletalMesh* MeshAsset = WeaponMesh->GetSkeletalMeshAsset();
		bool bHasMesh = MeshAsset != nullptr;
		bool bIsVisible = WeaponMesh->IsVisible();
		bool bIsHidden = Weapon->IsHidden();
		
		UE_LOG(LogTemp, Display, TEXT(">>> WEAPON ATTACH: Success! Mesh attached to %s"), *ThirdPersonWeaponSocket.ToString());
		UE_LOG(LogTemp, Display, TEXT(">>> WEAPON ATTACH: HasMeshAsset=%s, IsVisible=%s, IsActorHidden=%s"), 
			bHasMesh ? TEXT("YES") : TEXT("NO"),
			bIsVisible ? TEXT("YES") : TEXT("NO"),
			bIsHidden ? TEXT("YES") : TEXT("NO"));
		
		if (!bHasMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON ATTACH: ThirdPersonMesh has NO SkeletalMesh asset set!"));
		}
		
		// Check socket exists on character
		bool bSocketExists = GetMesh()->DoesSocketExist(ThirdPersonWeaponSocket);
		FVector SocketLocation = GetMesh()->GetSocketLocation(ThirdPersonWeaponSocket);
		FVector WeaponLocation = WeaponMesh->GetComponentLocation();
		
		UE_LOG(LogTemp, Display, TEXT(">>> WEAPON ATTACH: Socket '%s' exists on character: %s"), 
			*ThirdPersonWeaponSocket.ToString(), bSocketExists ? TEXT("YES") : TEXT("NO"));
		UE_LOG(LogTemp, Display, TEXT(">>> WEAPON ATTACH: Socket location: %s"), *SocketLocation.ToString());
		UE_LOG(LogTemp, Display, TEXT(">>> WEAPON ATTACH: Weapon location: %s"), *WeaponLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON ATTACH: ThirdPersonMesh component is null!"));
	}
}

void ABasePlayerCharacter::HolsterWeapon(ABaseWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// Determine holster socket based on weapon type
	FName HolsterSocket = NAME_None;
	switch (Weapon->GetHolsterType())
	{
	case EHolsterType::Hip:
		HolsterSocket = HolsterHipSocket;
		break;
	case EHolsterType::Back:
		HolsterSocket = HolsterBackSocket;
		break;
	case EHolsterType::None:
	default:
		// Hide weapon instead of holstering
		Weapon->SetActorHiddenInGame(true);
		return;
	}

	// Attach weapon to holster socket
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, HolsterSocket);
	Weapon->SetActorHiddenInGame(false);
	
	UE_LOG(LogTemp, Display, TEXT(">>> HOLSTER: Weapon %s holstered to %s"), *Weapon->GetName(), *HolsterSocket.ToString());
}

void ABasePlayerCharacter::PlayFiringMontage(UAnimMontage* Montage)
{
	// Stub - implement if needed
}

void ABasePlayerCharacter::AddWeaponRecoil(float Recoil)
{
	// Apply the recoil as pitch input
	AddControllerPitchInput(Recoil);
}

void ABasePlayerCharacter::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	if (WeaponComponent)
	{
		WeaponComponent->UpdateAmmoDisplay(CurrentAmmo, MagazineSize);
	}
}

FVector ABasePlayerCharacter::GetWeaponTargetLocation()
{
	if (!FollowCamera)
	{
		return GetActorLocation();
	}

	// Trace ahead from the camera viewpoint
	FHitResult OutHit;

	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + (FollowCamera->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	// Return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void ABasePlayerCharacter::AddWeaponClass(const TSubclassOf<ABaseWeapon>& WeaponClass)
{
	if (WeaponComponent)
	{
		WeaponComponent->AddWeaponClass(WeaponClass);
	}
}

void ABasePlayerCharacter::OnWeaponActivated(ABaseWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	// Set the character mesh AnimInstance (third-person only)
	if (!GetMesh())
	{
		return;
	}

	const TSubclassOf<UAnimInstance> WeaponAnimClass = Weapon->GetThirdPersonAnimInstanceClass();
	if (WeaponAnimClass)
	{
		GetMesh()->SetAnimInstanceClass(WeaponAnimClass);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Weapon [%s] has no ThirdPersonAnimInstanceClass - falling back"), *Weapon->GetName());
	if (UnarmedAnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(UnarmedAnimInstanceClass);
	}
}

void ABasePlayerCharacter::OnWeaponDeactivated(ABaseWeapon* Weapon)
{
	// Reset to unarmed animation (third-person only)
	if (UnarmedAnimInstanceClass && GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(UnarmedAnimInstanceClass);
	}
}

void ABasePlayerCharacter::OnSemiWeaponRefire()
{
	// Unused
}

// === GETTERS ===

bool ABasePlayerCharacter::IsDead() const
{
	return HealthComponent ? HealthComponent->IsDead() : false;
}

// === INTERACTION ===

void ABasePlayerCharacter::CheckForInteractable()
{
	if (!FollowCamera)
	{
		return;
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (FollowCamera->GetForwardVector() * InteractionDistance);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Use ECC_Pawn to hit characters (NPCs)
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, QueryParams);

	// Debug visualization
	if (bShowInteractionDebug)
	{
		if (bHit)
		{
			// Draw green line to hit point, then red line to end
			DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Green, false, 1.0f, 0, 3.0f);
			DrawDebugLine(GetWorld(), Hit.ImpactPoint, End, FColor::Red, false, 1.0f, 0, 2.0f);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 15.0f, 12, FColor::Yellow, false, 1.0f);
		}
		else
		{
			// Draw red line if no hit
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 3.0f);
		}
	}

	AActor* PreviousInteractable = CurrentInteractable;

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UQuestInteractable::StaticClass()))
		{
			CurrentInteractable = HitActor;
			
			if (bShowInteractionDebug)
			{
				UE_LOG(LogTemp, Display, TEXT(">>> INTERACTION DEBUG: Found interactable [%s] at distance %f cm"), 
					*HitActor->GetName(), Hit.Distance);
			}
			
			// Show prompt if this is a new interactable
			if (CurrentInteractable != PreviousInteractable)
			{
				// Hide previous prompt
				if (PreviousInteractable && PreviousInteractable->GetClass()->ImplementsInterface(UQuestInteractable::StaticClass()))
				{
					IQuestInteractable::Execute_HidePrompt(PreviousInteractable);
				}
				
				// Show new prompt
				IQuestInteractable::Execute_ShowPrompt(CurrentInteractable);
			}
			
			return;
		}
		else if (bShowInteractionDebug && HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> INTERACTION DEBUG: Hit [%s] but it doesn't implement IQuestInteractable"), 
				*HitActor->GetName());
		}
	}

	// No interactable found - hide previous prompt
	if (PreviousInteractable && PreviousInteractable->GetClass()->ImplementsInterface(UQuestInteractable::StaticClass()))
	{
		IQuestInteractable::Execute_HidePrompt(PreviousInteractable);
	}
	
	CurrentInteractable = nullptr;
}

void ABasePlayerCharacter::DoOpenJournal()
{
	if (UIManager && UIManager->GetQuestUIManager())
	{
		UIManager->GetQuestUIManager()->ToggleQuestJournal();
	}
}

void ABasePlayerCharacter::DoPauseMenu()
{
	if (UIManager)
	{
		UIManager->TogglePauseMenu();
	}
}

void ABasePlayerCharacter::DoToggleInventory()
{
	if (UIManager)
	{
		UIManager->ToggleInventory();
	}
}

void ABasePlayerCharacter::DoInteract()
{
	// Don't interact if quest dialog is open
	if (UIManager && UIManager->GetQuestUIManager() && UIManager->GetQuestUIManager()->IsQuestDialogOpen())
	{
		UE_LOG(LogTemp, Display, TEXT(">>> INTERACTION: Blocked - Quest dialog is open"));
		return;
	}

	// Check for interactable first
	CheckForInteractable();

	if (!CurrentInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> INTERACTION: No interactable found within %f cm"), InteractionDistance);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> INTERACTION: Player interacting with [%s]"), *CurrentInteractable->GetName());

	// Check if this is a quest giver
	if (UQuestGiverComponent* QuestGiver = CurrentInteractable->FindComponentByClass<UQuestGiverComponent>())
	{
		int32 QuestCount = QuestGiver->AvailableQuests.Num();
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Found QuestGiverComponent with %d available quest(s)"), QuestCount);
		
		if (!QuestGiver->OnQuestOffered.IsAlreadyBound(this, &ABasePlayerCharacter::HandleQuestOffered))
		{
			QuestGiver->OnQuestOffered.AddDynamic(this, &ABasePlayerCharacter::HandleQuestOffered);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Bound to OnQuestOffered delegate"));
		}
		
		if (!QuestGiver->OnQuestTurnedIn.IsAlreadyBound(this, &ABasePlayerCharacter::HandleQuestTurnedIn))
		{
			QuestGiver->OnQuestTurnedIn.AddDynamic(this, &ABasePlayerCharacter::HandleQuestTurnedIn);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Bound to OnQuestTurnedIn delegate"));
		}
	}

	// Call the interface function
	IQuestInteractable::Execute_Interact(CurrentInteractable, this);
}

void ABasePlayerCharacter::HandleQuestOffered(UQuestDefinition* Quest, AActor* QuestGiver)
{
	if (!Quest || !QuestGiver)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST OFFERED: Invalid Quest or QuestGiver!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST OFFERED: [%s] from [%s]"), 
		*Quest->QuestID.ToString(), *QuestGiver->GetName());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST TITLE: %s"), *Quest->UIData.QuestTitle.ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DESCRIPTION: %s"), *Quest->UIData.ShortDescription.ToString());

	// Show quest dialog widget
	ShowQuestDialog(Quest, QuestGiver);
	
	// Also call Blueprint event for custom handling
	OnQuestOfferedToPlayer(Quest, QuestGiver);
}

void ABasePlayerCharacter::HandleQuestTurnedIn(FName QuestID, UQuestDefinition* Quest, AActor* QuestGiver)
{
	if (!Quest || !QuestGiver)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST TURNED IN: Invalid Quest or QuestGiver!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST TURNED IN: [%s] from [%s]"), 
		*QuestID.ToString(), *QuestGiver->GetName());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST TITLE: %s"), *Quest->UIData.QuestTitle.ToString());

	// Show quest completion dialog widget
	ShowQuestCompletionDialog(Quest, Quest->Rewards, QuestGiver);
}

void ABasePlayerCharacter::ShowQuestDialog(UQuestDefinition* Quest, AActor* QuestGiver)
{
	// Delegate to QuestUIManager
	if (UIManager && UIManager->GetQuestUIManager())
	{
		UIManager->GetQuestUIManager()->ShowQuestGiverDialog(Quest, QuestGiver);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UI: UIManager or QuestUIManager is null"));
	}
}

void ABasePlayerCharacter::ShowQuestCompletionDialog(UQuestDefinition* Quest, const FQuestReward& Rewards, AActor* QuestGiver)
{
	// Delegate to QuestUIManager
	if (UIManager && UIManager->GetQuestUIManager())
	{
		UIManager->GetQuestUIManager()->ShowQuestCompletionDialog(Quest, Rewards, QuestGiver);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COMPLETION UI: UIManager or QuestUIManager is null"));
	}
}

float ABasePlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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
