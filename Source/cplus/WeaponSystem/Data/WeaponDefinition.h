// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponSystem/Data/WeaponTypes.h"
#include "WeaponDefinition.generated.h"

class ABaseWeapon;
class USkeletalMesh;
class UAnimMontage;
class USoundBase;
class UParticleSystem;
class UTexture2D;

/**
 * Weapon Definition DataAsset
 * 
 * Data-driven configuration for weapons.
 * Create instances in Content Browser for each weapon type.
 */
UCLASS(BlueprintType)
class CPLUS_API UWeaponDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UWeaponDefinition();

	// === IDENTITY ===

	/** Unique weapon identifier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	FName WeaponID;

	/** Display name for UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	FText DisplayName;

	/** Weapon category (affects slot assignment and behavior) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	EWeaponCategory Category = EWeaponCategory::Pistol;

	/** Weapon wheel slot (0-9, -1 for no slot) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity", meta = (ClampMin = -1, ClampMax = 9))
	int32 SlotIndex = 1;

	/** Icon for UI/HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	TObjectPtr<UTexture2D> Icon;

	// === COMBAT STATS ===

	/** Base damage per hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0))
	float Damage = 25.0f;

	/** Shots per second */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0.1f))
	float FireRate = 5.0f;

	/** Maximum effective range in cm */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0, Units = "cm"))
	float Range = 10000.0f;

	/** Fire mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	EWeaponFireMode FireMode = EWeaponFireMode::SemiAuto;

	/** Recoil amount per shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0))
	float Recoil = 1.0f;

	/** Spread in degrees (0 = perfectly accurate) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0, ClampMax = 45))
	float SpreadAngle = 0.0f;

	// === AMMO ===

	/** Ammo type (None = infinite/melee) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	EAmmoType AmmoType = EAmmoType::Pistol;

	/** Rounds per magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (ClampMin = 1))
	int32 MagazineSize = 12;

	/** Ammo consumed per shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (ClampMin = 1))
	int32 AmmoPerShot = 1;

	/** Reload time in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (ClampMin = 0))
	float ReloadTime = 2.0f;

	// === VISUALS ===

	/** Weapon actor class to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	TSubclassOf<ABaseWeapon> WeaponClass;

	/** First person mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	TObjectPtr<USkeletalMesh> FirstPersonMesh;

	/** Third person mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	TObjectPtr<USkeletalMesh> ThirdPersonMesh;

	// === ANIMATIONS ===

	/** Fire animation montage (first person) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> FireMontage_FP;

	/** Fire animation montage (third person) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> FireMontage_TP;

	/** Reload animation montage (first person) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> ReloadMontage_FP;

	/** Reload animation montage (third person) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> ReloadMontage_TP;

	/** Equip animation montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> EquipMontage;

	/** First person anim blueprint class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TSubclassOf<UAnimInstance> AnimInstance_FP;

	/** Third person anim blueprint class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TSubclassOf<UAnimInstance> AnimInstance_TP;

	// === AUDIO ===

	/** Fire sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> FireSound;

	/** Reload sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> ReloadSound;

	/** Empty clip sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> EmptySound;

	/** Equip sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> EquipSound;

	// === EFFECTS ===

	/** Muzzle flash effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	/** Shell eject effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ShellEject;

	// === HELPER FUNCTIONS ===

	/** Get time between shots based on fire rate */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetFireInterval() const { return 1.0f / FireRate; }

	/** Returns true if this weapon uses ammo */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool UsesAmmo() const { return AmmoType != EAmmoType::None; }

	/** Returns true if this is a melee weapon */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsMelee() const { return Category == EWeaponCategory::Melee || Category == EWeaponCategory::Unarmed; }
};
