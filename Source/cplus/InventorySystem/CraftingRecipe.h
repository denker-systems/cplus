// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CraftingRecipe.generated.h"

class UItemDefinition;

/**
 * Crafting Ingredient
 * 
 * Single ingredient requirement for a recipe
 */
USTRUCT(BlueprintType)
struct FCraftingIngredient
{
	GENERATED_BODY()

	/** Item required */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemDefinition> Item;

	/** Quantity required */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 Quantity = 1;
};

/**
 * Crafting Recipe DataAsset
 * 
 * Defines inputs and outputs for crafting
 */
UCLASS(BlueprintType)
class CPLUS_API UCraftingRecipe : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ===== IDENTITY =====

	/** Unique recipe identifier */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
	FName RecipeID;

	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity")
	FText RecipeName;

	/** Description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Identity", meta = (MultiLine = true))
	FText RecipeDescription;

	// ===== INPUTS =====

	/** Required ingredients */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Inputs")
	TArray<FCraftingIngredient> Ingredients;

	// ===== OUTPUTS =====

	/** Result item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Outputs")
	TObjectPtr<UItemDefinition> ResultItem;

	/** Result quantity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Outputs", meta = (ClampMin = 1))
	int32 ResultQuantity = 1;

	// ===== REQUIREMENTS =====

	/** Time to craft in seconds (0 = instant) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
	float CraftTime = 0.0f;

	/** Is this recipe unlocked by default? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
	bool bUnlockedByDefault = true;

	// ===== OVERRIDES =====

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CraftingRecipes", GetFName());
	}
};
