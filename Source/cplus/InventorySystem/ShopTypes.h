// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopTypes.generated.h"

class UItemDefinition;

/**
 * Shop Item Entry
 * Represents an item available for purchase in a shop
 */
USTRUCT(BlueprintType)
struct FShopItem
{
	GENERATED_BODY()

	/** The item for sale */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemDefinition> Item;

	/** Base price (can be modified by reputation, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 BasePrice = 100;

	/** Current stock (-1 = unlimited) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Stock = -1;

	/** Does stock replenish over time? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUnlimitedStock = true;

	/** Sell price multiplier (what vendor pays for this item) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0, ClampMax = 1.0))
	float SellPriceMultiplier = 0.5f;
};

/**
 * Shop Transaction Result
 */
UENUM(BlueprintType)
enum class EShopTransactionResult : uint8
{
	Success           UMETA(DisplayName = "Success"),
	NotEnoughGold     UMETA(DisplayName = "Not Enough Gold"),
	NotEnoughStock    UMETA(DisplayName = "Not Enough Stock"),
	ItemNotFound      UMETA(DisplayName = "Item Not Found"),
	InventoryFull     UMETA(DisplayName = "Inventory Full"),
	QuestItem         UMETA(DisplayName = "Cannot Sell Quest Item"),
	NotEnoughItems    UMETA(DisplayName = "Not Enough Items"),
	Failed            UMETA(DisplayName = "Failed")
};
