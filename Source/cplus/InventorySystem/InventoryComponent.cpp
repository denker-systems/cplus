// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "QuestSubSystem.h"
#include "WeaponSystem/Components/WeaponComponent.h"
#include "WeaponSystem/Actors/BaseWeapon.h"
#include "Core/HealthComponent.h"
#include "SaveSystem/SaveGameData.h"
#include "WorldItem.h"
#include "PickupComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// ===== ADD/REMOVE =====

int32 UInventoryComponent::AddItem(UItemDefinition* ItemData, int32 Quantity)
{
	if (!ItemData || Quantity <= 0)
	{
		return 0;
	}

	if (!CanAddItem(ItemData, Quantity))
	{
		OnInventoryFull.Broadcast(ItemData);
		return 0;
	}

	int32 AddedCount = 0;

	// Try to stack with existing items
	if (ItemData->MaxStackSize > 1)
	{
		for (FInventoryItem& Item : Items)
		{
			if (Item.ItemData == ItemData && Item.Quantity < ItemData->MaxStackSize)
			{
				int32 SpaceInStack = ItemData->MaxStackSize - Item.Quantity;
				int32 AmountToAdd = FMath::Min(SpaceInStack, Quantity);
				
				Item.Quantity += AmountToAdd;
				AddedCount += AmountToAdd;
				Quantity -= AmountToAdd;

				OnInventoryChanged.Broadcast(ItemData, Item.Quantity);

				if (Quantity <= 0)
				{
					break;
				}
			}
		}
	}

	// Create new stacks if needed
	while (Quantity > 0 && Items.Num() < MaxSlots)
	{
		FInventoryItem NewItem;
		NewItem.ItemData = ItemData;
		NewItem.Quantity = FMath::Min(Quantity, ItemData->MaxStackSize);
		
		Items.Add(NewItem);
		AddedCount += NewItem.Quantity;
		Quantity -= NewItem.Quantity;

		OnInventoryChanged.Broadcast(ItemData, NewItem.Quantity);
	}

	// Notify quest system
	if (AddedCount > 0)
	{
		NotifyQuestSystem(ItemData, AddedCount);
	}

	return AddedCount;
}

int32 UInventoryComponent::RemoveItem(UItemDefinition* ItemData, int32 Quantity)
{
	if (!ItemData || Quantity <= 0)
	{
		return 0;
	}

	int32 RemovedCount = 0;

	for (int32 i = Items.Num() - 1; i >= 0 && Quantity > 0; --i)
	{
		if (Items[i].ItemData == ItemData)
		{
			int32 AmountToRemove = FMath::Min(Items[i].Quantity, Quantity);
			Items[i].Quantity -= AmountToRemove;
			RemovedCount += AmountToRemove;
			Quantity -= AmountToRemove;

			if (Items[i].Quantity <= 0)
			{
				Items.RemoveAt(i);
			}

			OnInventoryChanged.Broadcast(ItemData, Items.Num() > i ? Items[i].Quantity : 0);
		}
	}

	return RemovedCount;
}

bool UInventoryComponent::RemoveItemByInstance(const FGuid& InstanceID)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].InstanceID == InstanceID)
		{
			UItemDefinition* ItemData = Items[i].ItemData;
			Items.RemoveAt(i);
			OnInventoryChanged.Broadcast(ItemData, 0);
			return true;
		}
	}
	return false;
}

// ===== QUERY =====

bool UInventoryComponent::HasItem(UItemDefinition* ItemData, int32 Quantity) const
{
	return GetItemQuantity(ItemData) >= Quantity;
}

int32 UInventoryComponent::GetItemQuantity(UItemDefinition* ItemData) const
{
	if (!ItemData)
	{
		return 0;
	}

	int32 TotalQuantity = 0;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemData == ItemData)
		{
			TotalQuantity += Item.Quantity;
		}
	}
	return TotalQuantity;
}

FInventoryItem UInventoryComponent::GetItemByID(FName ItemID) const
{
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemData && Item.ItemData->ItemID == ItemID)
		{
			return Item;
		}
	}
	return FInventoryItem();
}

TArray<FInventoryItem> UInventoryComponent::GetItemsByTag(FGameplayTag Tag) const
{
	TArray<FInventoryItem> MatchingItems;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemData && Item.ItemData->ItemTags.HasTag(Tag))
		{
			MatchingItems.Add(Item);
		}
	}
	return MatchingItems;
}

TArray<FInventoryItem> UInventoryComponent::GetItemsByType(EItemType ItemType) const
{
	TArray<FInventoryItem> MatchingItems;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemData && Item.ItemData->ItemType == ItemType)
		{
			MatchingItems.Add(Item);
		}
	}
	return MatchingItems;
}

float UInventoryComponent::GetCurrentWeight() const
{
	float TotalWeight = 0.0f;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemData)
		{
			TotalWeight += Item.ItemData->ItemWeight * Item.Quantity;
		}
	}
	return TotalWeight;
}

bool UInventoryComponent::HasSpace(UItemDefinition* ItemData, int32 Quantity) const
{
	return CanAddItem(ItemData, Quantity);
}

// ===== USE =====

bool UInventoryComponent::UseItem(UItemDefinition* ItemData)
{
	if (!ItemData || !ItemData->bIsUsable || !HasItem(ItemData, 1))
	{
		OnItemUsed.Broadcast(ItemData, false);
		return false;
	}

	// Apply item effects based on type
	AActor* Owner = GetOwner();
	if (Owner)
	{
		// Apply health restoration
		if (ItemData->HealthRestore > 0.0f)
		{
			UHealthComponent* HealthComp = Owner->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				HealthComp->Heal(ItemData->HealthRestore);
			}
		}

		// Apply stamina restoration (when stamina system exists)
		// if (ItemData->StaminaRestore > 0.0f)
		// {
		//     UStaminaComponent* StaminaComp = Owner->FindComponentByClass<UStaminaComponent>();
		//     if (StaminaComp)
		//     {
		//         StaminaComp->RestoreStamina(ItemData->StaminaRestore);
		//     }
		// }
	}

	// Consume item if needed
	if (ItemData->bConsumeOnUse)
	{
		RemoveItem(ItemData, 1);
	}

	OnItemUsed.Broadcast(ItemData, true);
	return true;
}

bool UInventoryComponent::DropItem(UItemDefinition* ItemData, int32 Quantity)
{
	if (!ItemData || !HasItem(ItemData, Quantity))
	{
		return false;
	}

	// Block quest items from being dropped
	if (ItemData->bIsQuestItem || !ItemData->bCanBeDropped)
	{
		return false;
	}

	// Get owner location for spawn
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	// Remove from inventory first
	int32 RemovedCount = RemoveItem(ItemData, Quantity);
	if (RemovedCount <= 0)
	{
		return false;
	}

	// Calculate spawn location (in front of owner)
	FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f;
	SpawnLocation.Z += 50.0f; // Slightly above ground
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// Spawn WorldItem
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AWorldItem* DroppedItem = World->SpawnActor<AWorldItem>(AWorldItem::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	if (DroppedItem && DroppedItem->PickupComponent)
	{
		DroppedItem->PickupComponent->ItemData = ItemData;
		DroppedItem->PickupComponent->ItemQuantity = RemovedCount;
	}

	return true;
}

bool UInventoryComponent::SellItem(UItemDefinition* ItemData, int32 Quantity, int32& OutValue)
{
	OutValue = 0;

	if (!ItemData || !HasItem(ItemData, Quantity))
	{
		return false;
	}

	// Block quest items from being sold
	if (ItemData->bIsQuestItem || !ItemData->bCanBeSold)
	{
		return false;
	}

	// Calculate sell value
	OutValue = ItemData->SellValue * Quantity;

	// Remove from inventory
	int32 RemovedCount = RemoveItem(ItemData, Quantity);

	return RemovedCount > 0;
}

bool UInventoryComponent::DestroyItem(UItemDefinition* ItemData, int32 Quantity)
{
	if (!ItemData || !HasItem(ItemData, Quantity))
	{
		return false;
	}

	// Block quest items from being destroyed
	if (ItemData->bIsQuestItem || !ItemData->bCanBeDestroyed)
	{
		return false;
	}

	// Remove from inventory (permanently)
	int32 RemovedCount = RemoveItem(ItemData, Quantity);

	return RemovedCount > 0;
}

// ===== SORTING =====

void UInventoryComponent::SortByName()
{
	Items.Sort([](const FInventoryItem& A, const FInventoryItem& B)
	{
		if (!A.ItemData || !B.ItemData) return false;
		return A.ItemData->ItemName.ToString() < B.ItemData->ItemName.ToString();
	});
}

void UInventoryComponent::SortByType()
{
	Items.Sort([](const FInventoryItem& A, const FInventoryItem& B)
	{
		if (!A.ItemData || !B.ItemData) return false;
		return A.ItemData->ItemType < B.ItemData->ItemType;
	});
}

void UInventoryComponent::SortByRarity()
{
	Items.Sort([](const FInventoryItem& A, const FInventoryItem& B)
	{
		if (!A.ItemData || !B.ItemData) return false;
		return A.ItemData->ItemRarity > B.ItemData->ItemRarity;
	});
}

// ===== QUEST INTEGRATION =====

bool UInventoryComponent::HasQuestItems(const TMap<FName, int32>& RequiredItems) const
{
	for (const auto& Pair : RequiredItems)
	{
		FInventoryItem Item = GetItemByID(Pair.Key);
		if (!Item.ItemData || Item.Quantity < Pair.Value)
		{
			return false;
		}
	}
	return true;
}

bool UInventoryComponent::ConsumeQuestItems(const TMap<FName, int32>& RequiredItems)
{
	if (!HasQuestItems(RequiredItems))
	{
		return false;
	}

	for (const auto& Pair : RequiredItems)
	{
		FInventoryItem Item = GetItemByID(Pair.Key);
		if (Item.ItemData)
		{
			RemoveItem(Item.ItemData, Pair.Value);
		}
	}

	return true;
}

// ===== PRIVATE HELPERS =====

int32 UInventoryComponent::FindItemIndex(UItemDefinition* ItemData) const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemData == ItemData)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

bool UInventoryComponent::CanAddItem(UItemDefinition* ItemData, int32 Quantity) const
{
	if (!ItemData || Quantity <= 0)
	{
		return false;
	}

	// Check weight limit
	if (bUseWeightLimit)
	{
		float NewWeight = GetCurrentWeight() + (ItemData->ItemWeight * Quantity);
		if (NewWeight > MaxWeight)
		{
			return false;
		}
	}

	// Check if can stack with existing items
	int32 RemainingQuantity = Quantity;
	if (ItemData->MaxStackSize > 1)
	{
		for (const FInventoryItem& Item : Items)
		{
			if (Item.ItemData == ItemData && Item.Quantity < ItemData->MaxStackSize)
			{
				int32 SpaceInStack = ItemData->MaxStackSize - Item.Quantity;
				RemainingQuantity -= SpaceInStack;
				if (RemainingQuantity <= 0)
				{
					return true;
				}
			}
		}
	}

	// Check if we have enough slots for new stacks
	int32 NeededSlots = FMath::CeilToInt(static_cast<float>(RemainingQuantity) / ItemData->MaxStackSize);
	int32 AvailableSlots = MaxSlots - Items.Num();
	
	return AvailableSlots >= NeededSlots;
}

void UInventoryComponent::NotifyQuestSystem(UItemDefinition* ItemData, int32 Quantity)
{
	if (!ItemData)
	{
		return;
	}

	// Get Quest Subsystem
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (QuestSubsystem)
	{
		// Notify quest system about collected items
		// NotifyQuestEvent takes: QuestID, EventTags, Instigator
		QuestSubsystem->NotifyQuestEvent(ItemData->ItemID, ItemData->ItemTags, GetOwner());
	}
}

// ===== WEAPON INTEGRATION =====

bool UInventoryComponent::EquipWeaponItem(UItemDefinition* WeaponItem)
{
	if (!WeaponItem)
	{
		return false;
	}

	// Verify this is a weapon item
	if (WeaponItem->ItemType != EItemType::Weapon)
	{
		return false;
	}

	// Verify we have this weapon in inventory
	if (!HasItem(WeaponItem, 1))
	{
		return false;
	}

	// Verify weapon class is set
	if (!WeaponItem->WeaponClass)
	{
		return false;
	}

	// Find weapon component on owner
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	UWeaponComponent* WeaponComp = Owner->FindComponentByClass<UWeaponComponent>();
	if (!WeaponComp)
	{
		return false;
	}

	// Unequip current weapon first if different
	if (EquippedWeaponItem && EquippedWeaponItem != WeaponItem)
	{
		UnequipWeaponItem();
	}

	// Add weapon to weapon component (handles spawning and equipping)
	WeaponComp->AddWeaponClass(WeaponItem->WeaponClass);

	// Track equipped weapon
	EquippedWeaponItem = WeaponItem;

	return true;
}

bool UInventoryComponent::UnequipWeaponItem()
{
	if (!EquippedWeaponItem)
	{
		return false;
	}

	// Find weapon component on owner
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	UWeaponComponent* WeaponComp = Owner->FindComponentByClass<UWeaponComponent>();
	if (WeaponComp)
	{
		// Remove weapon from weapon component
		WeaponComp->RemoveCurrentWeapon();
	}

	// Clear equipped tracking
	EquippedWeaponItem = nullptr;

	return true;
}

// ===== SAVE/LOAD =====

TArray<FInventoryItemSaveData> UInventoryComponent::GetSaveData() const
{
	TArray<FInventoryItemSaveData> SaveData;

	for (const FInventoryItem& Item : Items)
	{
		if (!Item.ItemData)
		{
			continue;
		}

		FInventoryItemSaveData ItemSave;
		ItemSave.ItemID = Item.ItemData->ItemID;
		ItemSave.Quantity = Item.Quantity;
		ItemSave.InstanceID = Item.InstanceID;

		SaveData.Add(ItemSave);
	}

	return SaveData;
}

void UInventoryComponent::LoadFromSaveData(const TArray<FInventoryItemSaveData>& SaveData)
{
	// Clear current inventory
	Items.Empty();

	// Get asset manager to load item definitions
	UAssetManager& AssetManager = UAssetManager::Get();

	for (const FInventoryItemSaveData& ItemSave : SaveData)
	{
		// Try to find item definition by ID
		FPrimaryAssetId AssetId = FPrimaryAssetId("Items", ItemSave.ItemID);
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);

		if (AssetPath.IsValid())
		{
			UItemDefinition* ItemDef = Cast<UItemDefinition>(AssetPath.TryLoad());
			if (ItemDef)
			{
				FInventoryItem NewItem;
				NewItem.ItemData = ItemDef;
				NewItem.Quantity = ItemSave.Quantity;
				NewItem.InstanceID = ItemSave.InstanceID;

				Items.Add(NewItem);
			}
		}
	}
}

