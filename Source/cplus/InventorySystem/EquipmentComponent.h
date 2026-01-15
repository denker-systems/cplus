// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentTypes.h"
#include "EquipmentComponent.generated.h"

class UItemDefinition;
class UInventoryComponent;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentChanged, EEquipmentSlot, Slot, UItemDefinition*, NewItem, UItemDefinition*, OldItem);

/**
 * Equipment Component
 * 
 * Manages equipped armor and accessories with stat modifiers.
 * Works alongside InventoryComponent - items stay in inventory but are marked as equipped.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	// ===== EVENTS =====

	/** Fired when equipment changes in any slot */
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

	// ===== API =====

	/**
	 * Equip item to slot
	 * @param Slot The equipment slot
	 * @param Item The item to equip (must be in inventory)
	 * @return True if successfully equipped
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipItem(EEquipmentSlot Slot, UItemDefinition* Item);

	/**
	 * Unequip item from slot
	 * @param Slot The equipment slot to clear
	 * @return The unequipped item, or nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UItemDefinition* UnequipItem(EEquipmentSlot Slot);

	/**
	 * Get equipped item in slot
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	UItemDefinition* GetEquippedItem(EEquipmentSlot Slot) const;

	/**
	 * Check if slot has equipped item
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool HasEquippedItem(EEquipmentSlot Slot) const;

	/**
	 * Get all equipped items
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	TMap<EEquipmentSlot, UItemDefinition*> GetAllEquippedItems() const
	{
		TMap<EEquipmentSlot, UItemDefinition*> Result;
		Result.Reserve(EquippedItems.Num());

		for (const auto& Pair : EquippedItems)
		{
			Result.Add(Pair.Key, Pair.Value.Get());
		}

		return Result;
	}

	/**
	 * Get total stat modifiers from all equipped items
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	FEquipmentStatModifier GetTotalStatModifiers() const;

	/**
	 * Get total armor rating
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	float GetTotalArmor() const;

protected:
	virtual void BeginPlay() override;

	/** Equipped items by slot */
	UPROPERTY()
	TMap<EEquipmentSlot, TObjectPtr<UItemDefinition>> EquippedItems;

	/** Cached inventory reference */
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Cache inventory component */
	void CacheInventoryComponent();

	/** Check if item can be equipped to slot */
	bool CanEquipToSlot(EEquipmentSlot Slot, UItemDefinition* Item) const;
};
