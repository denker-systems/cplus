// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopComponent.h"
#include "InventoryComponent.h"
#include "ItemDefinition.h"

UShopComponent::UShopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

EShopTransactionResult UShopComponent::BuyItem(UInventoryComponent* BuyerInventory, int32& BuyerGold, UItemDefinition* Item, int32 Quantity)
{
	if (!BuyerInventory || !Item || Quantity <= 0)
	{
		return EShopTransactionResult::Failed;
	}

	// Check if item is in shop
	const FShopItem* ShopItem = FindShopItem(Item);
	if (!ShopItem)
	{
		return EShopTransactionResult::ItemNotFound;
	}

	// Check stock
	if (!ShopItem->bUnlimitedStock && ShopItem->Stock < Quantity)
	{
		return EShopTransactionResult::NotEnoughStock;
	}

	// Calculate total cost
	int32 TotalCost = GetBuyPrice(Item, Quantity);

	// Check if buyer has enough gold
	if (BuyerGold < TotalCost)
	{
		return EShopTransactionResult::NotEnoughGold;
	}

	// Check if buyer has inventory space
	if (!BuyerInventory->HasSpace(Item, Quantity))
	{
		return EShopTransactionResult::InventoryFull;
	}

	// Perform transaction
	BuyerGold -= TotalCost;
	BuyerInventory->AddItem(Item, Quantity);

	// Reduce stock if not unlimited
	if (!ShopItem->bUnlimitedStock)
	{
		// Need non-const access to modify stock
		FShopItem* MutableShopItem = FindShopItem(Item);
		if (MutableShopItem)
		{
			MutableShopItem->Stock -= Quantity;
		}
	}

	OnItemPurchased.Broadcast(Item, Quantity, TotalCost);
	return EShopTransactionResult::Success;
}

int32 UShopComponent::GetBuyPrice(UItemDefinition* Item, int32 Quantity) const
{
	if (!Item || Quantity <= 0)
	{
		return 0;
	}

	const FShopItem* ShopItem = FindShopItem(Item);
	if (ShopItem)
	{
		return FMath::RoundToInt(ShopItem->BasePrice * Quantity * PriceModifier);
	}

	// Default to item's sell value * 2 if not in shop inventory
	return FMath::RoundToInt(Item->SellValue * 2.0f * Quantity * PriceModifier);
}

bool UShopComponent::IsInStock(UItemDefinition* Item, int32 Quantity) const
{
	const FShopItem* ShopItem = FindShopItem(Item);
	if (!ShopItem)
	{
		return false;
	}

	return ShopItem->bUnlimitedStock || ShopItem->Stock >= Quantity;
}

EShopTransactionResult UShopComponent::SellItem(UInventoryComponent* SellerInventory, int32& SellerGold, UItemDefinition* Item, int32 Quantity)
{
	if (!SellerInventory || !Item || Quantity <= 0)
	{
		return EShopTransactionResult::Failed;
	}

	// Quest items cannot be sold
	if (Item->bIsQuestItem || !Item->bCanBeSold)
	{
		return EShopTransactionResult::QuestItem;
	}

	// Check if seller has enough items
	if (!SellerInventory->HasItem(Item, Quantity))
	{
		return EShopTransactionResult::NotEnoughItems;
	}

	// Calculate sell value
	int32 TotalValue = GetSellPrice(Item, Quantity);

	// Perform transaction
	SellerInventory->RemoveItem(Item, Quantity);
	SellerGold += TotalValue;

	OnItemSold.Broadcast(Item, Quantity, TotalValue);
	return EShopTransactionResult::Success;
}

int32 UShopComponent::GetSellPrice(UItemDefinition* Item, int32 Quantity) const
{
	if (!Item || Quantity <= 0)
	{
		return 0;
	}

	// Check if shop has special sell price for this item
	const FShopItem* ShopItem = FindShopItem(Item);
	if (ShopItem)
	{
		return FMath::RoundToInt(ShopItem->BasePrice * ShopItem->SellPriceMultiplier * Quantity * SellPriceModifier);
	}

	// Default to item's sell value
	return FMath::RoundToInt(Item->SellValue * Quantity * SellPriceModifier);
}

bool UShopComponent::WillBuyItem(UItemDefinition* Item) const
{
	if (!Item)
	{
		return false;
	}

	// Won't buy quest items
	if (Item->bIsQuestItem || !Item->bCanBeSold)
	{
		return false;
	}

	// Shop buys all non-quest items by default
	return true;
}

TArray<FShopItem> UShopComponent::GetAvailableItems() const
{
	TArray<FShopItem> Available;
	for (const FShopItem& Item : ShopInventory)
	{
		if (Item.bUnlimitedStock || Item.Stock > 0)
		{
			Available.Add(Item);
		}
	}
	return Available;
}

FShopItem* UShopComponent::FindShopItem(UItemDefinition* Item)
{
	for (FShopItem& ShopItem : ShopInventory)
	{
		if (ShopItem.Item == Item)
		{
			return &ShopItem;
		}
	}
	return nullptr;
}

const FShopItem* UShopComponent::FindShopItem(UItemDefinition* Item) const
{
	for (const FShopItem& ShopItem : ShopInventory)
	{
		if (ShopItem.Item == Item)
		{
			return &ShopItem;
		}
	}
	return nullptr;
}
