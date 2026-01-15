// Copyright Epic Games, Inc. All Rights Reserved.


#include "WeaponSystem/Actors/BaseWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "WeaponSystem/Actors/BaseProjectile.h"
#include "WeaponSystem/IWeaponHolder.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// create the first person mesh
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);

	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->bOnlyOwnerSee = true;

	// create the third person mesh
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Third Person Mesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);

	ThirdPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	// Third person only - owner can see this mesh too
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// subscribe to the owner's destroyed delegate
	OwnerActor->OnDestroyed.AddDynamic(this, &ABaseWeapon::OnOwnerDestroyed);

	// cast the weapon owner
	WeaponOwner = Cast<IWeaponHolder>(OwnerActor);
	PawnOwner = Cast<APawn>(OwnerActor);
	if (!WeaponOwner)
	{
		return;
	}

	// fill the first ammo clip
	CurrentBullets = MagazineSize;

	// attach the meshes to the owner
	WeaponOwner->AttachWeaponMeshes(this);
}

void ABaseWeapon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear timers
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void ABaseWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	// ensure this weapon is destroyed when the owner is destroyed
	Destroy();
}

void ABaseWeapon::ActivateWeapon()
{
	// unhide this weapon
	SetActorHiddenInGame(false);

	// notify the owner
	WeaponOwner->OnWeaponActivated(this);
}

void ABaseWeapon::DeactivateWeapon()
{
	// ensure we're no longer firing this weapon while deactivated
	StopFiring();

	// hide the weapon
	SetActorHiddenInGame(true);

	// notify the owner
	WeaponOwner->OnWeaponDeactivated(this);
}

void ABaseWeapon::StartFiring()
{
	// raise the firing flag
	bIsFiring = true;

	// check how much time has passed since we last shot
	// this may be under the refire rate if the weapon shoots slow enough and the player is spamming the trigger
	const float TimeSinceLastShot = GetWorld()->GetTimeSeconds() - TimeOfLastShot;

	if (TimeSinceLastShot > RefireRate)
	{
		// fire the weapon right away
		Fire();

	} else {

		// if we're full auto, schedule the next shot
		if (bFullAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &ABaseWeapon::Fire, TimeSinceLastShot, false);
		}

	}
}

void ABaseWeapon::StopFiring()
{
	// lower the firing flag
	bIsFiring = false;

	// clear the refire timer
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void ABaseWeapon::Fire()
{
	// ensure the player still wants to fire. They may have let go of the trigger
	if (!bIsFiring)
	{
		return;
	}
	
	// fire a projectile at the target
	FireProjectile(WeaponOwner->GetWeaponTargetLocation());

	// spawn muzzle flash effect
	if (MuzzleFlashEffect)
	{
		FVector MuzzleLocation = ThirdPersonMesh->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = ThirdPersonMesh->GetSocketRotation(MuzzleSocketName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, MuzzleLocation, MuzzleRotation, FVector(MuzzleFlashScale), true);
	}

	// update the time of our last shot
	TimeOfLastShot = GetWorld()->GetTimeSeconds();

	// make noise so the AI perception system can hear us
	MakeNoise(ShotLoudness, PawnOwner, PawnOwner->GetActorLocation(), ShotNoiseRange, ShotNoiseTag);

	// are we full auto?
	if (bFullAuto)
	{
		// schedule the next shot
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &ABaseWeapon::Fire, RefireRate, false);
	} else {

		// for semi-auto weapons, schedule the cooldown notification
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &ABaseWeapon::FireCooldownExpired, RefireRate, false);

	}
}

void ABaseWeapon::FireCooldownExpired()
{
	// notify the owner
	WeaponOwner->OnSemiWeaponRefire();
}

void ABaseWeapon::FireProjectile(const FVector& TargetLocation)
{
	// get the projectile transform
	FTransform ProjectileTransform = CalculateProjectileSpawnTransform(TargetLocation);
	
	// Debug visualization
	if (bShowDebugWeapon)
	{
		// Get muzzle location for debug (same logic as CalculateProjectileSpawnTransform)
		FVector MuzzleLoc;
		FString UsedMesh = "None";
		
		if (FirstPersonMesh && FirstPersonMesh->DoesSocketExist(MuzzleSocketName))
		{
			MuzzleLoc = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);
			UsedMesh = "FirstPerson";
		}
		else if (ThirdPersonMesh && ThirdPersonMesh->DoesSocketExist(MuzzleSocketName))
		{
			MuzzleLoc = ThirdPersonMesh->GetSocketLocation(MuzzleSocketName);
			UsedMesh = "ThirdPerson";
		}
		else
		{
			MuzzleLoc = GetActorLocation();
			UsedMesh = "WeaponRoot (SOCKET NOT FOUND!)";
		}
		
		const FVector SpawnLoc = ProjectileTransform.GetLocation();
		const FVector AimDir = ProjectileTransform.GetRotation().GetForwardVector();
		
		// Draw muzzle socket (yellow sphere)
		DrawDebugSphere(GetWorld(), MuzzleLoc, 5.0f, 8, FColor::Yellow, false, DebugLineDuration, 0, DebugLineThickness);
		
		// Draw spawn point (green sphere)
		DrawDebugSphere(GetWorld(), SpawnLoc, 8.0f, 8, FColor::Green, false, DebugLineDuration, 0, DebugLineThickness);
		
		// Draw aim direction (cyan line to target)
		DrawDebugLine(GetWorld(), SpawnLoc, TargetLocation, FColor::Cyan, false, DebugLineDuration, 0, DebugLineThickness);
		
		// Draw actual projectile direction (orange line)
		const FVector ProjectileEnd = SpawnLoc + (AimDir * 5000.0f);
		DrawDebugLine(GetWorld(), SpawnLoc, ProjectileEnd, FColor::Orange, false, DebugLineDuration, 0, DebugLineThickness);
		
		// Draw aim variance cone (if any)
		if (AimVariance > 0.0f)
		{
			DrawDebugCone(GetWorld(), SpawnLoc, AimDir, 1000.0f, FMath::DegreesToRadians(AimVariance), FMath::DegreesToRadians(AimVariance), 16, FColor::Red, false, DebugLineDuration, 0, DebugLineThickness * 0.5f);
		}
		
		UE_LOG(LogTemp, Display, TEXT(">>> WEAPON DEBUG: Fire - Mesh: %s, Socket: %s, Muzzle: %s, Spawn: %s"), 
			*UsedMesh, *MuzzleSocketName.ToString(), *MuzzleLoc.ToString(), *SpawnLoc.ToString());
	}
	
	// spawn the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = PawnOwner;

	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileTransform, SpawnParams);

	// play the firing montage
	WeaponOwner->PlayFiringMontage(FiringMontage);

	// add recoil
	WeaponOwner->AddWeaponRecoil(FiringRecoil);

	// Consume bullets
	--CurrentBullets;

	// Update the weapon HUD
	WeaponOwner->UpdateWeaponHUD(CurrentBullets, MagazineSize);

	// If clip is depleted, stop firing (WeaponComponent will handle reload)
	if (CurrentBullets <= 0)
	{
		StopFiring();
	}
}

FTransform ABaseWeapon::CalculateProjectileSpawnTransform(const FVector& TargetLocation) const
{
	// Try to find the muzzle socket location
	FVector MuzzleLoc;
	
	// Check if socket exists on FirstPersonMesh
	if (FirstPersonMesh && FirstPersonMesh->DoesSocketExist(MuzzleSocketName))
	{
		MuzzleLoc = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);
	}
	// Fallback to ThirdPersonMesh if FirstPerson socket doesn't exist
	else if (ThirdPersonMesh && ThirdPersonMesh->DoesSocketExist(MuzzleSocketName))
	{
		MuzzleLoc = ThirdPersonMesh->GetSocketLocation(MuzzleSocketName);
		if (bShowDebugWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON DEBUG: Using ThirdPersonMesh socket (FirstPerson socket not found)"));
		}
	}
	// Last resort: use weapon forward + offset (approximates muzzle)
	else
	{
		// Calculate muzzle position from weapon's forward direction
		// This works even without a socket - assumes barrel points forward
		MuzzleLoc = GetActorLocation() + (GetActorForwardVector() * 50.0f) + FVector(0, 0, 10.0f);
		if (bShowDebugWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON DEBUG: MuzzleSocketName '%s' not found! Using calculated muzzle position. Consider adding socket to weapon mesh."), *MuzzleSocketName.ToString());
		}
	}

	// calculate the spawn location ahead of the muzzle
	const FVector SpawnLoc = MuzzleLoc + ((TargetLocation - MuzzleLoc).GetSafeNormal() * MuzzleOffset);

	// find the aim rotation vector while applying some variance to the target 
	const FRotator AimRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * AimVariance));

	// return the built transform
	return FTransform(AimRot, SpawnLoc, FVector::OneVector);
}

const TSubclassOf<UAnimInstance>& ABaseWeapon::GetFirstPersonAnimInstanceClass() const
{
	return FirstPersonAnimInstanceClass;
}

const TSubclassOf<UAnimInstance>& ABaseWeapon::GetThirdPersonAnimInstanceClass() const
{
	return ThirdPersonAnimInstanceClass;
}

void ABaseWeapon::AddBullets(int32 Amount)
{
	CurrentBullets = FMath::Min(CurrentBullets + Amount, MagazineSize);

	// Update HUD
	if (WeaponOwner)
	{
		WeaponOwner->UpdateWeaponHUD(CurrentBullets, MagazineSize);
	}
}

void ABaseWeapon::StartReload()
{
	if (bIsReloading || CurrentBullets >= MagazineSize)
	{
		return;
	}

	// Stop firing while reloading
	StopFiring();

	bIsReloading = true;

	// Schedule reload completion
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&ABaseWeapon::FinishReload,
		ReloadTime,
		false
	);
}

void ABaseWeapon::CancelReload()
{
	if (bIsReloading)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		bIsReloading = false;
	}
}

void ABaseWeapon::FinishReload()
{
	bIsReloading = false;
	// Note: Actual ammo addition is handled by WeaponComponent
}
