// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "InventoryItem.generated.h"

/**
 * Inventory Item Instance
 * 
 * Runtime representation of an item in inventory.
 * Contains reference to ItemDefinition plus instance-specific data.
 */
USTRUCT(BlueprintType)
struct CPLUS_API FInventoryItem
{
	GENERATED_BODY()

	/** Reference to item definition (data) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UItemDefinition> ItemData;

	/** Stack count */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Quantity = 1;

	/** Unique instance ID (for tracking specific items) */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", Meta = (IgnoreForMemberInitializationTest))
	FGuid InstanceID;

	/** Custom data (for weapon durability, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FName, float> CustomData;

	/** Timestamp when acquired */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", Meta = (IgnoreForMemberInitializationTest))
	FDateTime AcquiredTime;

	// Constructor
	FInventoryItem()
	{
		ItemData = nullptr;
		Quantity = 1;
		InstanceID = FGuid::NewGuid();
		AcquiredTime = FDateTime::Now();
	}

	/** Check if this item can stack with another */
	bool CanStackWith(const FInventoryItem& Other) const
	{
		return ItemData == Other.ItemData && 
		       ItemData && 
		       ItemData->MaxStackSize > 1;
	}

	/** Get display name with quantity */
	FText GetDisplayName() const
	{
		if (ItemData)
		{
			if (Quantity > 1)
			{
				return FText::Format(FText::FromString("{0} ({1})"), 
				                   ItemData->ItemName, 
				                   FText::AsNumber(Quantity));
			}
			return ItemData->ItemName;
		}
		return FText::FromString("Unknown Item");
	}
};
