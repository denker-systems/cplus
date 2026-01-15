// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingComponent.generated.h"

class UCraftingRecipe;
class UInventoryComponent;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftingStarted, UCraftingRecipe*, Recipe, float, CraftTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingCompleted, UCraftingRecipe*, Recipe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingFailed, UCraftingRecipe*, Recipe);

/**
 * Crafting Component
 * 
 * Handles crafting using recipes and inventory materials.
 * Attach to player character.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCraftingComponent();

	// ===== EVENTS =====

	UPROPERTY(BlueprintAssignable, Category = "Crafting")
	FOnCraftingStarted OnCraftingStarted;

	UPROPERTY(BlueprintAssignable, Category = "Crafting")
	FOnCraftingCompleted OnCraftingCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Crafting")
	FOnCraftingFailed OnCraftingFailed;

	// ===== STATE =====

	/** Is currently crafting? */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	bool bIsCrafting = false;

	/** Current recipe being crafted */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<UCraftingRecipe> CurrentRecipe;

	// ===== UNLOCKED RECIPES =====

	/** Known/unlocked recipes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TArray<TObjectPtr<UCraftingRecipe>> UnlockedRecipes;

	// ===== API =====

	/**
	 * Check if can craft recipe
	 * @param Recipe The recipe to check
	 * @return True if has all ingredients and recipe is unlocked
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool CanCraft(UCraftingRecipe* Recipe) const;

	/**
	 * Start crafting a recipe
	 * @param Recipe The recipe to craft
	 * @return True if crafting started
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool StartCrafting(UCraftingRecipe* Recipe);

	/**
	 * Cancel current crafting
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CancelCrafting();

	/**
	 * Unlock a recipe
	 * @param Recipe The recipe to unlock
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void UnlockRecipe(UCraftingRecipe* Recipe);

	/**
	 * Check if recipe is unlocked
	 * @param Recipe The recipe to check
	 * @return True if unlocked
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool IsRecipeUnlocked(UCraftingRecipe* Recipe) const;

	/**
	 * Get all craftable recipes (unlocked + has ingredients)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	TArray<UCraftingRecipe*> GetCraftableRecipes() const;

protected:
	virtual void BeginPlay() override;

	/** Cached inventory reference */
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Timer for craft completion */
	FTimerHandle CraftTimerHandle;

	/** Consume ingredients from inventory */
	bool ConsumeIngredients(UCraftingRecipe* Recipe);

	/** Complete crafting and add result to inventory */
	void FinishCrafting();

	/** Cache inventory component */
	void CacheInventoryComponent();
};
