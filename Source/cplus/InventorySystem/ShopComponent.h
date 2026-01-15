// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShopTypes.h"
#include "ShopComponent.generated.h"

class UItemDefinition;
class UInventoryComponent;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemPurchased, UItemDefinition*, Item, int32, Quantity, int32, TotalCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemSold, UItemDefinition*, Item, int32, Quantity, int32, TotalValue);

/**
 * Shop Component
 * 
 * Attach to NPC vendors to enable buying/selling items.
 * Handles inventory transactions, pricing, and stock management.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShopComponent();

	// ===== EVENTS =====

	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnItemPurchased OnItemPurchased;

	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnItemSold OnItemSold;

	// ===== SHOP INVENTORY =====

	/** Items this shop sells */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	TArray<FShopItem> ShopInventory;

	/** Price modifier (1.0 = normal, 0.8 = 20% discount) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (ClampMin = 0.1, ClampMax = 5.0))
	float PriceModifier = 1.0f;

	/** Sell price modifier (what shop pays for items) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (ClampMin = 0.1, ClampMax = 1.0))
	float SellPriceModifier = 0.5f;

	// ===== BUY API =====

	/**
	 * Buy item from shop
	 * @param BuyerInventory The buyer's inventory component
	 * @param BuyerGold Reference to buyer's gold (will be modified)
	 * @param Item The item to buy
	 * @param Quantity How many to buy
	 * @return Transaction result
	 */
	UFUNCTION(BlueprintCallable, Category = "Shop")
	EShopTransactionResult BuyItem(UInventoryComponent* BuyerInventory, UPARAM(ref) int32& BuyerGold, UItemDefinition* Item, int32 Quantity = 1);

	/**
	 * Get buy price for item
	 */
	UFUNCTION(BlueprintPure, Category = "Shop")
	int32 GetBuyPrice(UItemDefinition* Item, int32 Quantity = 1) const;

	/**
	 * Check if item is in stock
	 */
	UFUNCTION(BlueprintPure, Category = "Shop")
	bool IsInStock(UItemDefinition* Item, int32 Quantity = 1) const;

	// ===== SELL API =====

	/**
	 * Sell item to shop
	 * @param SellerInventory The seller's inventory component
	 * @param SellerGold Reference to seller's gold (will be modified)
	 * @param Item The item to sell
	 * @param Quantity How many to sell
	 * @return Transaction result
	 */
	UFUNCTION(BlueprintCallable, Category = "Shop")
	EShopTransactionResult SellItem(UInventoryComponent* SellerInventory, UPARAM(ref) int32& SellerGold, UItemDefinition* Item, int32 Quantity = 1);

	/**
	 * Get sell price for item (what shop pays)
	 */
	UFUNCTION(BlueprintPure, Category = "Shop")
	int32 GetSellPrice(UItemDefinition* Item, int32 Quantity = 1) const;

	/**
	 * Check if shop will buy this item
	 */
	UFUNCTION(BlueprintPure, Category = "Shop")
	bool WillBuyItem(UItemDefinition* Item) const;

	// ===== UTILITY =====

	/**
	 * Get available items for purchase
	 */
	UFUNCTION(BlueprintPure, Category = "Shop")
	TArray<FShopItem> GetAvailableItems() const;

protected:
	/** Find shop item entry */
	FShopItem* FindShopItem(UItemDefinition* Item);
	const FShopItem* FindShopItem(UItemDefinition* Item) const;
};
