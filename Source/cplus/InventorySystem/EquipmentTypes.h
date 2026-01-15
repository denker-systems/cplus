// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentTypes.generated.h"

/**
 * Equipment Slot Enumeration
 * Defines available slots for equipping armor/accessories
 */
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None        UMETA(DisplayName = "None"),
	Head        UMETA(DisplayName = "Head"),
	Chest       UMETA(DisplayName = "Chest"),
	Legs        UMETA(DisplayName = "Legs"),
	Hands       UMETA(DisplayName = "Hands"),
	Feet        UMETA(DisplayName = "Feet"),
	Accessory1  UMETA(DisplayName = "Accessory 1"),
	Accessory2  UMETA(DisplayName = "Accessory 2")
};

/**
 * Equipment Stat Modifier
 * Defines stat bonuses from equipped items
 */
USTRUCT(BlueprintType)
struct FEquipmentStatModifier
{
	GENERATED_BODY()

	/** Armor rating bonus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArmorBonus = 0.0f;

	/** Max health bonus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealthBonus = 0.0f;

	/** Movement speed modifier (1.0 = normal) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeedModifier = 1.0f;

	/** Damage resistance (0.0 - 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageResistance = 0.0f;
};
