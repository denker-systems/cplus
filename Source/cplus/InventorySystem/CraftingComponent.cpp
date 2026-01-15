// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingComponent.h"
#include "CraftingRecipe.h"
#include "InventoryComponent.h"
#include "ItemDefinition.h"
#include "TimerManager.h"

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheInventoryComponent();
}

void UCraftingComponent::CacheInventoryComponent()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		InventoryComponent = Owner->FindComponentByClass<UInventoryComponent>();
	}
}

bool UCraftingComponent::CanCraft(UCraftingRecipe* Recipe) const
{
	if (!Recipe || !IsValid(InventoryComponent))
	{
		return false;
	}

	// Check if recipe is unlocked
	if (!IsRecipeUnlocked(Recipe))
	{
		return false;
	}

	// Check if currently crafting
	if (bIsCrafting)
	{
		return false;
	}

	// Check if has all ingredients
	for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
	{
		if (!Ingredient.Item)
		{
			continue;
		}

		if (!InventoryComponent->HasItem(Ingredient.Item, Ingredient.Quantity))
		{
			return false;
		}
	}

	// Check if has space for result
	if (Recipe->ResultItem)
	{
		if (!InventoryComponent->HasSpace(Recipe->ResultItem, Recipe->ResultQuantity))
		{
			return false;
		}
	}

	return true;
}

bool UCraftingComponent::StartCrafting(UCraftingRecipe* Recipe)
{
	if (!CanCraft(Recipe))
	{
		OnCraftingFailed.Broadcast(Recipe);
		return false;
	}

	// Consume ingredients first
	if (!ConsumeIngredients(Recipe))
	{
		OnCraftingFailed.Broadcast(Recipe);
		return false;
	}

	CurrentRecipe = Recipe;
	bIsCrafting = true;

	OnCraftingStarted.Broadcast(Recipe, Recipe->CraftTime);

	// Instant craft or timed?
	if (Recipe->CraftTime <= 0.0f)
	{
		FinishCrafting();
	}
	else
	{
		// Set timer for completion
		GetWorld()->GetTimerManager().SetTimer(
			CraftTimerHandle,
			this,
			&UCraftingComponent::FinishCrafting,
			Recipe->CraftTime,
			false
		);
	}

	return true;
}

void UCraftingComponent::CancelCrafting()
{
	if (!bIsCrafting)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(CraftTimerHandle);

	// Note: Ingredients already consumed, not refunded on cancel
	// Could add refund logic here if desired

	bIsCrafting = false;
	CurrentRecipe = nullptr;
}

void UCraftingComponent::UnlockRecipe(UCraftingRecipe* Recipe)
{
	if (Recipe && !UnlockedRecipes.Contains(Recipe))
	{
		UnlockedRecipes.Add(Recipe);
	}
}

bool UCraftingComponent::IsRecipeUnlocked(UCraftingRecipe* Recipe) const
{
	if (!Recipe)
	{
		return false;
	}

	// Check if unlocked by default
	if (Recipe->bUnlockedByDefault)
	{
		return true;
	}

	return UnlockedRecipes.Contains(Recipe);
}

TArray<UCraftingRecipe*> UCraftingComponent::GetCraftableRecipes() const
{
	TArray<UCraftingRecipe*> Result;

	for (UCraftingRecipe* Recipe : UnlockedRecipes)
	{
		if (CanCraft(Recipe))
		{
			Result.Add(Recipe);
		}
	}

	return Result;
}

bool UCraftingComponent::ConsumeIngredients(UCraftingRecipe* Recipe)
{
	if (!Recipe || !IsValid(InventoryComponent))
	{
		return false;
	}

	// Consume each ingredient
	for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
	{
		if (!Ingredient.Item)
		{
			continue;
		}

		int32 Removed = InventoryComponent->RemoveItem(Ingredient.Item, Ingredient.Quantity);
		if (Removed < Ingredient.Quantity)
		{
			// Failed to consume (shouldn't happen if CanCraft passed)
			return false;
		}
	}

	return true;
}

void UCraftingComponent::FinishCrafting()
{
	if (!CurrentRecipe || !IsValid(InventoryComponent))
	{
		bIsCrafting = false;
		CurrentRecipe = nullptr;
		return;
	}

	// Add result to inventory
	if (CurrentRecipe->ResultItem)
	{
		InventoryComponent->AddItem(CurrentRecipe->ResultItem, CurrentRecipe->ResultQuantity);
	}

	OnCraftingCompleted.Broadcast(CurrentRecipe);

	bIsCrafting = false;
	CurrentRecipe = nullptr;
}
