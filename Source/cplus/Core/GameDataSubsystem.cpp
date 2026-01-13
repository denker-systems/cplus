// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDataSubsystem.h"
#include "ItemDefinition.h"
#include "QuestDefinition.h"
#include "InteractableDefinition.h"
#include "Engine/AssetManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameData, Log, All);

void UGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogGameData, Log, TEXT("GameDataSubsystem initializing..."));
	
	// Load all assets on startup
	LoadAllAssets();
	
	UE_LOG(LogGameData, Log, TEXT("GameDataSubsystem initialized: %d Items, %d Quests, %d Interactables"),
		ItemCache.Num(), QuestCache.Num(), InteractableCache.Num());
}

void UGameDataSubsystem::Deinitialize()
{
	UE_LOG(LogGameData, Log, TEXT("GameDataSubsystem deinitializing..."));
	
	// Clear caches
	ItemCache.Empty();
	QuestCache.Empty();
	InteractableCache.Empty();
	
	Super::Deinitialize();
}

void UGameDataSubsystem::LoadAllAssets()
{
	CacheItems();
	CacheQuests();
	CacheInteractables();
}

void UGameDataSubsystem::CacheItems()
{
	ItemCache.Empty();
	
	UAssetManager& AssetManager = UAssetManager::Get();
	
	// Get all primary assets of type "Items"
	TArray<FPrimaryAssetId> ItemAssetIds;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("Items"), ItemAssetIds);
	
	for (const FPrimaryAssetId& AssetId : ItemAssetIds)
	{
		// Synchronously load the asset
		UObject* LoadedAsset = AssetManager.GetPrimaryAssetObject(AssetId);
		
		if (!LoadedAsset)
		{
			// Try synchronous load if not already loaded
			FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);
			LoadedAsset = AssetPath.TryLoad();
		}
		
		if (UItemDefinition* Item = Cast<UItemDefinition>(LoadedAsset))
		{
			ItemCache.Add(Item->ItemID, Item);
			UE_LOG(LogGameData, Verbose, TEXT("Cached Item: %s"), *Item->ItemID.ToString());
		}
	}
	
	UE_LOG(LogGameData, Log, TEXT("Cached %d items"), ItemCache.Num());
}

void UGameDataSubsystem::CacheQuests()
{
	QuestCache.Empty();
	
	UAssetManager& AssetManager = UAssetManager::Get();
	
	// Get all primary assets of type "Quests"
	TArray<FPrimaryAssetId> QuestAssetIds;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("Quests"), QuestAssetIds);
	
	for (const FPrimaryAssetId& AssetId : QuestAssetIds)
	{
		UObject* LoadedAsset = AssetManager.GetPrimaryAssetObject(AssetId);
		
		if (!LoadedAsset)
		{
			FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);
			LoadedAsset = AssetPath.TryLoad();
		}
		
		if (UQuestDefinition* Quest = Cast<UQuestDefinition>(LoadedAsset))
		{
			QuestCache.Add(Quest->QuestID, Quest);
			UE_LOG(LogGameData, Verbose, TEXT("Cached Quest: %s"), *Quest->QuestID.ToString());
		}
	}
	
	UE_LOG(LogGameData, Log, TEXT("Cached %d quests"), QuestCache.Num());
}

void UGameDataSubsystem::CacheInteractables()
{
	InteractableCache.Empty();
	
	UAssetManager& AssetManager = UAssetManager::Get();
	
	// Get all primary assets of type "Interactables"
	TArray<FPrimaryAssetId> InteractableAssetIds;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("Interactables"), InteractableAssetIds);
	
	for (const FPrimaryAssetId& AssetId : InteractableAssetIds)
	{
		UObject* LoadedAsset = AssetManager.GetPrimaryAssetObject(AssetId);
		
		if (!LoadedAsset)
		{
			FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);
			LoadedAsset = AssetPath.TryLoad();
		}
		
		if (UInteractableDefinition* Interactable = Cast<UInteractableDefinition>(LoadedAsset))
		{
			InteractableCache.Add(Interactable->InteractableID, Interactable);
			UE_LOG(LogGameData, Verbose, TEXT("Cached Interactable: %s"), *Interactable->InteractableID.ToString());
		}
	}
	
	UE_LOG(LogGameData, Log, TEXT("Cached %d interactables"), InteractableCache.Num());
}

// ===== ITEM API =====

UItemDefinition* UGameDataSubsystem::GetItemByID(FName ItemID) const
{
	if (const TObjectPtr<UItemDefinition>* Found = ItemCache.Find(ItemID))
	{
		return *Found;
	}
	
	UE_LOG(LogGameData, Warning, TEXT("Item not found: %s"), *ItemID.ToString());
	return nullptr;
}

TArray<UItemDefinition*> UGameDataSubsystem::GetAllItems() const
{
	TArray<UItemDefinition*> Result;
	for (const auto& Pair : ItemCache)
	{
		if (Pair.Value)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

TArray<UItemDefinition*> UGameDataSubsystem::GetItemsByTag(FGameplayTag Tag) const
{
	TArray<UItemDefinition*> Result;
	
	for (const auto& Pair : ItemCache)
	{
		if (Pair.Value && Pair.Value->ItemTags.HasTag(Tag))
		{
			Result.Add(Pair.Value);
		}
	}
	
	return Result;
}

TArray<UItemDefinition*> UGameDataSubsystem::GetItemsByTags(const FGameplayTagContainer& Tags) const
{
	TArray<UItemDefinition*> Result;
	
	for (const auto& Pair : ItemCache)
	{
		if (Pair.Value && Pair.Value->ItemTags.HasAny(Tags))
		{
			Result.Add(Pair.Value);
		}
	}
	
	return Result;
}

// ===== QUEST API =====

UQuestDefinition* UGameDataSubsystem::GetQuestByID(FName QuestID) const
{
	if (const TObjectPtr<UQuestDefinition>* Found = QuestCache.Find(QuestID))
	{
		return *Found;
	}
	
	UE_LOG(LogGameData, Warning, TEXT("Quest not found: %s"), *QuestID.ToString());
	return nullptr;
}

TArray<UQuestDefinition*> UGameDataSubsystem::GetAllQuests() const
{
	TArray<UQuestDefinition*> Result;
	for (const auto& Pair : QuestCache)
	{
		if (Pair.Value)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

// ===== INTERACTABLE API =====

UInteractableDefinition* UGameDataSubsystem::GetInteractableByID(FName InteractableID) const
{
	if (const TObjectPtr<UInteractableDefinition>* Found = InteractableCache.Find(InteractableID))
	{
		return *Found;
	}
	
	UE_LOG(LogGameData, Warning, TEXT("Interactable not found: %s"), *InteractableID.ToString());
	return nullptr;
}

TArray<UInteractableDefinition*> UGameDataSubsystem::GetAllInteractables() const
{
	TArray<UInteractableDefinition*> Result;
	for (const auto& Pair : InteractableCache)
	{
		if (Pair.Value)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

// ===== DEBUG =====

void UGameDataSubsystem::LogAllRegisteredAssets() const
{
	UE_LOG(LogGameData, Log, TEXT("========== GAME DATA SUBSYSTEM =========="));
	
	UE_LOG(LogGameData, Log, TEXT("--- ITEMS (%d) ---"), ItemCache.Num());
	for (const auto& Pair : ItemCache)
	{
		if (Pair.Value)
		{
			UE_LOG(LogGameData, Log, TEXT("  [%s] %s"), *Pair.Key.ToString(), *Pair.Value->ItemName.ToString());
		}
	}
	
	UE_LOG(LogGameData, Log, TEXT("--- QUESTS (%d) ---"), QuestCache.Num());
	for (const auto& Pair : QuestCache)
	{
		if (Pair.Value)
		{
			UE_LOG(LogGameData, Log, TEXT("  [%s] %s"), *Pair.Key.ToString(), *Pair.Value->UIData.QuestTitle.ToString());
		}
	}
	
	UE_LOG(LogGameData, Log, TEXT("--- INTERACTABLES (%d) ---"), InteractableCache.Num());
	for (const auto& Pair : InteractableCache)
	{
		if (Pair.Value)
		{
			UE_LOG(LogGameData, Log, TEXT("  [%s] %s"), *Pair.Key.ToString(), *Pair.Value->InteractionPrompt.ToString());
		}
	}
	
	UE_LOG(LogGameData, Log, TEXT("=========================================="));
}

bool UGameDataSubsystem::ValidateAllAssets() const
{
	bool bAllValid = true;
	
	UE_LOG(LogGameData, Log, TEXT("Validating all game data assets..."));
	
	// Validate items
	for (const auto& Pair : ItemCache)
	{
		if (!Pair.Value)
		{
			UE_LOG(LogGameData, Error, TEXT("Invalid Item reference: %s"), *Pair.Key.ToString());
			bAllValid = false;
		}
		else if (Pair.Value->ItemID.IsNone())
		{
			UE_LOG(LogGameData, Warning, TEXT("Item has empty ItemID: %s"), *Pair.Value->GetName());
			bAllValid = false;
		}
	}
	
	// Validate quests
	for (const auto& Pair : QuestCache)
	{
		if (!Pair.Value)
		{
			UE_LOG(LogGameData, Error, TEXT("Invalid Quest reference: %s"), *Pair.Key.ToString());
			bAllValid = false;
		}
		else if (Pair.Value->QuestID.IsNone())
		{
			UE_LOG(LogGameData, Warning, TEXT("Quest has empty QuestID: %s"), *Pair.Value->GetName());
			bAllValid = false;
		}
	}
	
	// Validate interactables
	for (const auto& Pair : InteractableCache)
	{
		if (!Pair.Value)
		{
			UE_LOG(LogGameData, Error, TEXT("Invalid Interactable reference: %s"), *Pair.Key.ToString());
			bAllValid = false;
		}
	}
	
	if (bAllValid)
	{
		UE_LOG(LogGameData, Log, TEXT("All assets validated successfully!"));
	}
	else
	{
		UE_LOG(LogGameData, Warning, TEXT("Some assets failed validation!"));
	}
	
	return bAllValid;
}

void UGameDataSubsystem::ReloadAllAssets()
{
	UE_LOG(LogGameData, Log, TEXT("Reloading all game data assets..."));
	LoadAllAssets();
	UE_LOG(LogGameData, Log, TEXT("Reload complete: %d Items, %d Quests, %d Interactables"),
		ItemCache.Num(), QuestCache.Num(), InteractableCache.Num());
}
