// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameDataSubsystem.generated.h"

// Forward declarations
class UItemDefinition;
class UQuestDefinition;
class UInteractableDefinition;

/**
 * Game Data Subsystem
 * 
 * Centralized manager for all game data assets (Items, Quests, Interactables).
 * Uses UAssetManager for automatic discovery and async loading.
 * Provides convenient API for accessing data assets by ID or tag.
 * 
 * Access: GetGameInstance()->GetSubsystem<UGameDataSubsystem>()
 * 
 * @see UItemDefinition
 * @see UQuestDefinition
 * @see UInteractableDefinition
 */
UCLASS()
class CPLUS_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// ===== LIFECYCLE =====
	
	/** Called when subsystem is initialized */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/** Called when subsystem is deinitialized */
	virtual void Deinitialize() override;
	
	/** Should this subsystem be created? Always true for game data */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	// ===== ITEM API =====
	
	/**
	 * Get item definition by ID
	 * @param ItemID The unique item identifier (e.g., "DA_Item_HealthPotion")
	 * @return Item definition or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Items")
	UItemDefinition* GetItemByID(FName ItemID) const;
	
	/**
	 * Get all registered item definitions
	 * @return Array of all item definitions
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Items")
	TArray<UItemDefinition*> GetAllItems() const;
	
	/**
	 * Get items that have a specific gameplay tag
	 * @param Tag The tag to filter by
	 * @return Array of items with matching tag
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Items")
	TArray<UItemDefinition*> GetItemsByTag(FGameplayTag Tag) const;
	
	/**
	 * Get items that have any of the specified tags
	 * @param Tags Container of tags to match
	 * @return Array of items with any matching tag
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Items")
	TArray<UItemDefinition*> GetItemsByTags(const FGameplayTagContainer& Tags) const;

	// ===== QUEST API =====
	
	/**
	 * Get quest definition by ID
	 * @param QuestID The unique quest identifier
	 * @return Quest definition or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Quests")
	UQuestDefinition* GetQuestByID(FName QuestID) const;
	
	/**
	 * Get all registered quest definitions
	 * @return Array of all quest definitions
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Quests")
	TArray<UQuestDefinition*> GetAllQuests() const;

	// ===== INTERACTABLE API =====
	
	/**
	 * Get interactable definition by ID
	 * @param InteractableID The unique interactable identifier
	 * @return Interactable definition or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Interactables")
	UInteractableDefinition* GetInteractableByID(FName InteractableID) const;
	
	/**
	 * Get all registered interactable definitions
	 * @return Array of all interactable definitions
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Interactables")
	TArray<UInteractableDefinition*> GetAllInteractables() const;

	// ===== STATISTICS =====
	
	/**
	 * Get total count of registered items
	 */
	UFUNCTION(BlueprintPure, Category = "Game Data|Stats")
	int32 GetItemCount() const { return ItemCache.Num(); }
	
	/**
	 * Get total count of registered quests
	 */
	UFUNCTION(BlueprintPure, Category = "Game Data|Stats")
	int32 GetQuestCount() const { return QuestCache.Num(); }
	
	/**
	 * Get total count of registered interactables
	 */
	UFUNCTION(BlueprintPure, Category = "Game Data|Stats")
	int32 GetInteractableCount() const { return InteractableCache.Num(); }

	// ===== DEBUG =====
	
	/**
	 * Log all registered assets to output log
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Debug")
	void LogAllRegisteredAssets() const;
	
	/**
	 * Validate all asset references
	 * @return True if all assets are valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Debug")
	bool ValidateAllAssets() const;
	
	/**
	 * Reload all assets from AssetManager (useful for hot reload)
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Data|Debug")
	void ReloadAllAssets();

private:
	// ===== CACHES =====
	
	/** Cached item definitions, keyed by ItemID */
	UPROPERTY()
	TMap<FName, TObjectPtr<UItemDefinition>> ItemCache;
	
	/** Cached quest definitions, keyed by QuestID */
	UPROPERTY()
	TMap<FName, TObjectPtr<UQuestDefinition>> QuestCache;
	
	/** Cached interactable definitions, keyed by InteractableID */
	UPROPERTY()
	TMap<FName, TObjectPtr<UInteractableDefinition>> InteractableCache;

	// ===== INTERNAL =====
	
	/** Load and cache all assets */
	void LoadAllAssets();
	
	/** Load and cache items from AssetManager */
	void CacheItems();
	
	/** Load and cache quests from AssetManager */
	void CacheQuests();
	
	/** Load and cache interactables from AssetManager */
	void CacheInteractables();
};
