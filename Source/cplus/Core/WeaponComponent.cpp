#include "WeaponComponent.h"
#include "ShooterWeapon.h"
#include "ShooterWeaponHolder.h"
#include "GameFramework/Character.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponComponent::AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	if (!WeaponClass)
	{
		return;
	}

	// Do we already own this weapon?
	AShooterWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);
	if (OwnedWeapon)
	{
		return;
	}

	// Get owner actor
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// Spawn the new weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Cast<APawn>(Owner);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	AShooterWeapon* AddedWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, Owner->GetActorTransform(), SpawnParams);

	if (AddedWeapon)
	{
		// Add the weapon to the owned list
		OwnedWeapons.Add(AddedWeapon);

		// If we have an existing weapon, deactivate it
		if (CurrentWeapon)
		{
			CurrentWeapon->DeactivateWeapon();
		}

		// Switch to the new weapon
		AShooterWeapon* OldWeapon = CurrentWeapon;
		CurrentWeapon = AddedWeapon;
		CurrentWeapon->ActivateWeapon();

		// Broadcast weapon changed
		OnWeaponChanged.Broadcast(CurrentWeapon, OldWeapon);
	}
}

void UWeaponComponent::SwitchWeapon()
{
	// Ensure we have at least two weapons to switch between
	if (OwnedWeapons.Num() <= 1 || !CurrentWeapon)
	{
		return;
	}

	// Deactivate the old weapon
	CurrentWeapon->DeactivateWeapon();

	// Find the index of the current weapon in the owned list
	int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);

	// Is this the last weapon?
	if (WeaponIndex == OwnedWeapons.Num() - 1)
	{
		// Loop back to the beginning of the array
		WeaponIndex = 0;
	}
	else
	{
		// Select the next weapon index
		++WeaponIndex;
	}

	// Set the new weapon as current
	AShooterWeapon* OldWeapon = CurrentWeapon;
	CurrentWeapon = OwnedWeapons[WeaponIndex];

	// Activate the new weapon
	CurrentWeapon->ActivateWeapon();

	// Broadcast weapon changed
	OnWeaponChanged.Broadcast(CurrentWeapon, OldWeapon);
}

void UWeaponComponent::StartFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFiring();
	}
}

void UWeaponComponent::StopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

TArray<AShooterWeapon*> UWeaponComponent::GetOwnedWeapons() const
{
	TArray<AShooterWeapon*> Result;
	for (AShooterWeapon* Weapon : OwnedWeapons)
	{
		Result.Add(Weapon);
	}
	return Result;
}

AShooterWeapon* UWeaponComponent::FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const
{
	// Check each owned weapon
	for (AShooterWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon && Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	// Weapon not found
	return nullptr;
}

void UWeaponComponent::DeactivateCurrentWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DeactivateWeapon();
	}
}

void UWeaponComponent::UpdateAmmoDisplay(int32 CurrentAmmo, int32 MagazineSize)
{
	OnAmmoChanged.Broadcast(CurrentAmmo, MagazineSize);
}
