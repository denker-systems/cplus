// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ItemDefinition.generated.h"

/**
 * Item Type Enumeration
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable  UMETA(DisplayName = "Consumable"),
	Weapon      UMETA(DisplayName = "Weapon"),
	Armor       UMETA(DisplayName = "Armor"),
	Ammo        UMETA(DisplayName = "Ammo"),
	QuestItem   UMETA(DisplayName = "Quest Item"),
	Material    UMETA(DisplayName = "Material"),
	Misc        UMETA(DisplayName = "Miscellaneous")
};

/**
 * Item Rarity Enumeration
 */
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common      UMETA(DisplayName = "Common"),
	Uncommon    UMETA(DisplayName = "Uncommon"),
	Rare        UMETA(DisplayName = "Rare"),
	Epic        UMETA(DisplayName = "Epic"),
	Legendary   UMETA(DisplayName = "Legendary")
};

/**
 * Item Definition DataAsset
 * 
 * Defines all properties of an item (data only, no logic).
 * Create instances of this in the Content Browser to define items.
 */
UCLASS(BlueprintType)
class CPLUS_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ===== IDENTITY =====
	
	/** Unique item identifier */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
	FName ItemID;

	/** Type of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
	EItemType ItemType;

	/** Rarity of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
	EItemRarity ItemRarity;

	// ===== UI DATA =====
	
	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	FText ItemName;

	/** Description shown in UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI", meta = (MultiLine = true))
	FText ItemDescription;

	/** Icon for inventory display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	TObjectPtr<UTexture2D> ItemIcon;

	/** Color based on rarity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	FLinearColor RarityColor;

	// ===== GAMEPLAY TAGS =====
	
	/** Tags for categorization and quest matching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Tags")
	FGameplayTagContainer ItemTags;

	// ===== INVENTORY PROPERTIES =====
	
	/** Maximum stack size (1 = no stacking) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Inventory")
	int32 MaxStackSize = 1;

	/** Weight of single item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Inventory")
	float ItemWeight = 0.0f;

	/** Sell value in currency */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Inventory")
	int32 SellValue = 0;

	/** Can this item be dropped? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Inventory")
	bool bCanBeDropped = true;

	/** Can this item be sold? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Inventory")
	bool bCanBeSold = true;

	/** Can this item be destroyed? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Inventory")
	bool bCanBeDestroyed = true;

	// ===== QUEST INTEGRATION =====
	
	/** Is this a quest item? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Quest")
	bool bIsQuestItem = false;

	/** Related quest IDs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Quest", meta = (EditCondition = "bIsQuestItem"))
	TArray<FName> RelatedQuests;

	/** Consumed when quest completes? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Quest")
	bool bConsumedOnQuestComplete = false;

	// ===== USABILITY =====
	
	/** Can this item be used? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Usage")
	bool bIsUsable = false;

	/** Consume item on use? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Usage", meta = (EditCondition = "bIsUsable"))
	bool bConsumeOnUse = true;

	// ===== VISUAL REPRESENTATION =====
	
	/** Mesh for world representation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visual")
	TObjectPtr<UStaticMesh> WorldMesh;

	/** Mesh for inventory/character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visual")
	TObjectPtr<USkeletalMesh> InventoryMesh;

	/** Scale for world mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visual")
	FVector WorldMeshScale = FVector(1.0f);

	// ===== EFFECTS (for consumables) =====
	
	/** Health restored on use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	float HealthRestore = 0.0f;

	/** Stamina restored on use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	float StaminaRestore = 0.0f;

	// ===== OVERRIDES =====
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Items", GetFName());
	}
};
