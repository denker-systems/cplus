// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

/**
 * Weapon category for slot assignment and behavior
 */
UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	Unarmed		UMETA(DisplayName = "Unarmed"),
	Melee		UMETA(DisplayName = "Melee"),
	Pistol		UMETA(DisplayName = "Pistol"),
	SMG			UMETA(DisplayName = "SMG"),
	Rifle		UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun"),
	Sniper		UMETA(DisplayName = "Sniper"),
	Heavy		UMETA(DisplayName = "Heavy"),
	Explosive	UMETA(DisplayName = "Explosive"),
	Special		UMETA(DisplayName = "Special")
};

/**
 * Ammo type for inventory management
 */
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	None		UMETA(DisplayName = "None (Infinite/Melee)"),
	Pistol		UMETA(DisplayName = "Pistol Ammo"),
	Rifle		UMETA(DisplayName = "Rifle Ammo"),
	Shotgun		UMETA(DisplayName = "Shotgun Shells"),
	Sniper		UMETA(DisplayName = "Sniper Rounds"),
	Explosive	UMETA(DisplayName = "Explosives"),
	Energy		UMETA(DisplayName = "Energy Cells"),
	Special		UMETA(DisplayName = "Special Ammo")
};

/**
 * Current weapon state
 */
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Firing		UMETA(DisplayName = "Firing"),
	Reloading	UMETA(DisplayName = "Reloading"),
	Switching	UMETA(DisplayName = "Switching"),
	Disabled	UMETA(DisplayName = "Disabled")
};

/**
 * Weapon fire mode
 */
UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	SemiAuto	UMETA(DisplayName = "Semi-Automatic"),
	FullAuto	UMETA(DisplayName = "Full-Automatic"),
	Burst		UMETA(DisplayName = "Burst Fire"),
	Charge		UMETA(DisplayName = "Charge Shot")
};
