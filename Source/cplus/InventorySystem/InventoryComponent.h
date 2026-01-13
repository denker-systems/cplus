// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "InventoryComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, UItemDefinition*, Item, int32, NewQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryFull, UItemDefinition*, AttemptedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, UItemDefinition*, Item, bool, Success);


/**
 * Inventory Component
 * 
 * Manages item storage and inventory operations.
 * Attach to player character or AI companions.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// ===== INVENTORY SETTINGS =====
	
	/** Maximum number of inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots = 20;

	/** Maximum weight capacity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	float MaxWeight = 100.0f;

	/** Use weight limit? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bUseWeightLimit = false;

	// ===== INVENTORY DATA =====
	
	/** Actual inventory storage */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryItem> Items;

	// ===== EVENTS =====
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryFull OnInventoryFull;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemUsed OnItemUsed;

	// ===== API - ADD/REMOVE =====
	
	/**
	 * Add item to inventory
	 * @param ItemData The item to add
	 * @param Quantity How many to add
	 * @return Number of items actually added
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddItem(UItemDefinition* ItemData, int32 Quantity = 1);

	/**
	 * Remove item from inventory
	 * @param ItemData The item to remove
	 * @param Quantity How many to remove
	 * @return Number of items actually removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 RemoveItem(UItemDefinition* ItemData, int32 Quantity = 1);

	/**
	 * Remove item by instance ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByInstance(const FGuid& InstanceID);

	// ===== API - QUERY =====
	
	/**
	 * Check if inventory has item
	 * @param ItemData The item to check
	 * @param Quantity Minimum quantity required
	 * @return True if has enough
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItem(UItemDefinition* ItemData, int32 Quantity = 1) const;

	/**
	 * Get quantity of specific item
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetItemQuantity(UItemDefinition* ItemData) const;

	/**
	 * Get item by ID
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventoryItem GetItemByID(FName ItemID) const;

	/**
	 * Get all items with specific tag
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<FInventoryItem> GetItemsByTag(FGameplayTag Tag) const;

	/**
	 * Get current inventory weight
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	float GetCurrentWeight() const;

	/**
	 * Check if inventory has space
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasSpace(UItemDefinition* ItemData, int32 Quantity = 1) const;

	// ===== API - USE =====
	
	/**
	 * Use item from inventory
	 * @param ItemData The item to use
	 * @return True if successfully used
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(UItemDefinition* ItemData);

	/**
	 * Drop item from inventory
	 * @param ItemData The item to drop
	 * @param Quantity How many to drop
	 * @return True if successfully dropped
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItem(UItemDefinition* ItemData, int32 Quantity = 1);

	// ===== API - SORTING =====
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortByName();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortByType();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortByRarity();

	// ===== QUEST INTEGRATION =====
	
	/**
	 * Check if has items for quest
	 * @param RequiredItems Map of ItemID -> Quantity
	 * @return True if has all required items
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Quest")
	bool HasQuestItems(const TMap<FName, int32>& RequiredItems) const;

	/**
	 * Consume items for quest
	 * @param RequiredItems Map of ItemID -> Quantity
	 * @return True if successfully consumed
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Quest")
	bool ConsumeQuestItems(const TMap<FName, int32>& RequiredItems);

protected:
	virtual void BeginPlay() override;

	int32 FindItemIndex(UItemDefinition* ItemData) const;
	bool CanAddItem(UItemDefinition* ItemData, int32 Quantity) const;
	void NotifyQuestSystem(UItemDefinition* ItemData, int32 Quantity);
};
