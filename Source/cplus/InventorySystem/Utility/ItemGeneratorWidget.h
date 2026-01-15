#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "InventorySystem/ItemDefinition.h"
#include "ItemGeneratorWidget.generated.h"

/**
 * Item Generator Widget
 * 
 * Editor Utility Widget for generating test item definitions.
 * Creates ItemDefinition DataAssets for testing the inventory system.
 */
UCLASS()
class CPLUS_API UItemGeneratorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	// ===== GENERATION SETTINGS =====

	/** Number of items per type to generate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation", meta = (ClampMin = 1, ClampMax = 10))
	int32 ItemsPerType = 2;

	/** Prefix for item names */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation")
	FString ItemPrefix = TEXT("Test");

	/** Output folder path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation")
	FString OutputFolder = TEXT("/Game/Plugin/Items/Generated");

	// ===== TYPE TOGGLES =====

	/** Generate consumable items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation|Types")
	bool bGenerateConsumables = true;

	/** Generate weapon items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation|Types")
	bool bGenerateWeapons = true;

	/** Generate ammo items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation|Types")
	bool bGenerateAmmo = true;

	/** Generate quest items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation|Types")
	bool bGenerateQuestItems = true;

	/** Generate material items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation|Types")
	bool bGenerateMaterials = true;

	/** Generate misc items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Generation|Types")
	bool bGenerateMisc = true;

	// ===== BLUEPRINT GENERATION =====

	/** Base WorldItem class to create children from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint Generation")
	TSubclassOf<class AWorldItem> WorldItemClass;

	/** Output folder for generated blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint Generation")
	FString BlueprintOutputFolder = TEXT("/Game/Plugin/Items/Blueprints");

	// ===== API =====

	/** Generate test items (DataAssets only) */
	UFUNCTION(BlueprintCallable, Category = "Item Generation")
	void GenerateItems();

	/** Generate WorldItem Blueprints for all items in output folder */
	UFUNCTION(BlueprintCallable, Category = "Item Generation")
	void GenerateWorldItemBlueprints();

	/** Clear generated items list */
	UFUNCTION(BlueprintCallable, Category = "Item Generation")
	void ClearGeneratedItems();

	/** Get list of generated item names */
	UFUNCTION(BlueprintPure, Category = "Item Generation")
	TArray<FString> GetGeneratedItemNames() const { return GeneratedItemNames; }

private:
	/** Create a single item */
	UItemDefinition* CreateItem(EItemType Type, int32 Index);

	/** Get rarity color */
	FLinearColor GetRarityColor(EItemRarity Rarity) const;

	/** List of generated item names */
	UPROPERTY()
	TArray<FString> GeneratedItemNames;
};
