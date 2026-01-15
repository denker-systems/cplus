// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponHolder.generated.h"

class ABaseWeapon;
class UAnimMontage;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Weapon Holder Interface
 * 
 *  Common interface for characters that can hold weapons.
 */
class CPLUS_API IWeaponHolder
{
	GENERATED_BODY()

public:

	/** Attaches a weapon's meshes to the owner */
	virtual void AttachWeaponMeshes(ABaseWeapon* Weapon) = 0;

	/** Plays the firing montage for the weapon */
	virtual void PlayFiringMontage(UAnimMontage* Montage) = 0;

	/** Applies weapon recoil to the owner */
	virtual void AddWeaponRecoil(float Recoil) = 0;

	/** Updates the weapon's HUD with the current ammo count */
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) = 0;

	/** Calculates and returns the aim location for the weapon */
	virtual FVector GetWeaponTargetLocation() = 0;

	/** Gives a weapon of this class to the owner */
	virtual void AddWeaponClass(const TSubclassOf<ABaseWeapon>& WeaponClass) = 0;

	/** Activates the passed weapon */
	virtual void OnWeaponActivated(ABaseWeapon* Weapon) = 0;

	/** Deactivates the passed weapon */
	virtual void OnWeaponDeactivated(ABaseWeapon* Weapon) = 0;

	/** Notifies the owner that the weapon cooldown has expired and it's ready to shoot again */
	virtual void OnSemiWeaponRefire() = 0;
};
