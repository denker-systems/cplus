#include "BasePlayerCharacter.h"
#include "QuestTrackerComponent.h"
#include "InventoryComponent.h"
#include "InteractionComponent.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "ShooterWeapon.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Constructor called"));
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure the character mesh
	GetMesh()->SetOwnerNoSee(true);

	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create the spotlight for horror mode
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(FirstPersonCameraComponent);
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
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Disable tick by default
	PrimaryActorTick.bCanEverTick = false;
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: BeginPlay called for %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Location = %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Camera = %s"), FirstPersonCameraComponent ? TEXT("Valid") : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: Mesh = %s"), FirstPersonMesh ? TEXT("Valid") : TEXT("NULL"));

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
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::DoStartFiring);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::DoStopFiring);
		}

		// Switch weapon
		if (SwitchWeaponAction)
		{
			EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::DoSwitchWeapon);
		}

		// Sprinting
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::DoEndSprint);
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
	if (Yaw != 0.0f || Pitch != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("BasePlayerCharacter: DoAim called - Yaw=%f, Pitch=%f"), Yaw, Pitch);
	}
	
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
}

void ABasePlayerCharacter::DoJumpStart()
{
	if (!IsDead())
	{
		Jump();
	}
}

void ABasePlayerCharacter::DoJumpEnd()
{
	if (!IsDead())
	{
		StopJumping();
	}
}

void ABasePlayerCharacter::DoStartFiring()
{
	if (WeaponComponent && !IsDead())
	{
		WeaponComponent->StartFiring();
	}
}

void ABasePlayerCharacter::DoStopFiring()
{
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

void ABasePlayerCharacter::DoStartSprint()
{
	bSprinting = true;

	if (!bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		OnSprintStateChanged.Broadcast(true);
	}
}

void ABasePlayerCharacter::DoEndSprint()
{
	bSprinting = false;

	if (!bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		OnSprintStateChanged.Broadcast(false);
	}
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

// === IShooterWeaponHolder INTERFACE ===

void ABasePlayerCharacter::AttachWeaponMeshes(AShooterWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// Attach the weapon actor
	Weapon->AttachToActor(this, AttachmentRule);

	// Attach the weapon meshes
	Weapon->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, ThirdPersonWeaponSocket);
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
	if (!FirstPersonCameraComponent)
	{
		return GetActorLocation();
	}

	// Trace ahead from the camera viewpoint
	FHitResult OutHit;

	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	// Return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void ABasePlayerCharacter::AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	if (WeaponComponent)
	{
		WeaponComponent->AddWeaponClass(WeaponClass);
	}
}

void ABasePlayerCharacter::OnWeaponActivated(AShooterWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	// Set the character mesh AnimInstances
	if (FirstPersonMesh)
	{
		FirstPersonMesh->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	}
	if (GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
	}
}

void ABasePlayerCharacter::OnWeaponDeactivated(AShooterWeapon* Weapon)
{
	// Unused
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
