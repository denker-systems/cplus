// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentComponent.h"
#include "InventoryComponent.h"
#include "ItemDefinition.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheInventoryComponent();
}

void UEquipmentComponent::CacheInventoryComponent()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		InventoryComponent = Owner->FindComponentByClass<UInventoryComponent>();
	}
}

bool UEquipmentComponent::EquipItem(EEquipmentSlot Slot, UItemDefinition* Item)
{
	if (Slot == EEquipmentSlot::None || !Item)
	{
		return false;
	}

	// Verify item can be equipped to this slot
	if (!CanEquipToSlot(Slot, Item))
	{
		return false;
	}

	// Verify item is in inventory
	if (IsValid(InventoryComponent) && !InventoryComponent->HasItem(Item, 1))
	{
		return false;
	}

	// Unequip current item in slot first
	UItemDefinition* OldItem = nullptr;
	if (EquippedItems.Contains(Slot))
	{
		OldItem = EquippedItems[Slot];
	}

	// Equip new item
	EquippedItems.Add(Slot, Item);

	// Broadcast change
	OnEquipmentChanged.Broadcast(Slot, Item, OldItem);

	return true;
}

UItemDefinition* UEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
	if (Slot == EEquipmentSlot::None)
	{
		return nullptr;
	}

	UItemDefinition* OldItem = nullptr;
	if (EquippedItems.Contains(Slot))
	{
		OldItem = EquippedItems[Slot];
		EquippedItems.Remove(Slot);

		// Broadcast change
		OnEquipmentChanged.Broadcast(Slot, nullptr, OldItem);
	}

	return OldItem;
}

UItemDefinition* UEquipmentComponent::GetEquippedItem(EEquipmentSlot Slot) const
{
	const TObjectPtr<UItemDefinition>* Found = EquippedItems.Find(Slot);
	return Found ? *Found : nullptr;
}

bool UEquipmentComponent::HasEquippedItem(EEquipmentSlot Slot) const
{
	return EquippedItems.Contains(Slot) && IsValid(EquippedItems[Slot]);
}

FEquipmentStatModifier UEquipmentComponent::GetTotalStatModifiers() const
{
	FEquipmentStatModifier Total;
	Total.MovementSpeedModifier = 1.0f;

	for (const auto& Pair : EquippedItems)
	{
		if (!IsValid(Pair.Value))
		{
			continue;
		}

		// Get stats from item (would need EquipmentStats property in ItemDefinition)
		// For now, use placeholder based on item type
		if (Pair.Value->ItemType == EItemType::Armor)
		{
			// Base armor from sell value as placeholder
			Total.ArmorBonus += Pair.Value->SellValue * 0.1f;
		}
	}

	return Total;
}

float UEquipmentComponent::GetTotalArmor() const
{
	return GetTotalStatModifiers().ArmorBonus;
}

bool UEquipmentComponent::CanEquipToSlot(EEquipmentSlot Slot, UItemDefinition* Item) const
{
	if (!Item)
	{
		return false;
	}

	// Only armor items can be equipped
	if (Item->ItemType != EItemType::Armor)
	{
		return false;
	}

	// Check if item has matching slot tag
	// Expected tags: Item.Armor.Head, Item.Armor.Chest, etc.
	FGameplayTag SlotTag;
	switch (Slot)
	{
		case EEquipmentSlot::Head:
			SlotTag = FGameplayTag::RequestGameplayTag(FName("Item.Armor.Head"), false);
			break;
		case EEquipmentSlot::Chest:
			SlotTag = FGameplayTag::RequestGameplayTag(FName("Item.Armor.Chest"), false);
			break;
		case EEquipmentSlot::Legs:
			SlotTag = FGameplayTag::RequestGameplayTag(FName("Item.Armor.Legs"), false);
			break;
		case EEquipmentSlot::Hands:
			SlotTag = FGameplayTag::RequestGameplayTag(FName("Item.Armor.Hands"), false);
			break;
		case EEquipmentSlot::Feet:
			SlotTag = FGameplayTag::RequestGameplayTag(FName("Item.Armor.Feet"), false);
			break;
		case EEquipmentSlot::Accessory1:
		case EEquipmentSlot::Accessory2:
			// Accessories don't require specific tag
			return true;
		default:
			return false;
	}

	// If slot tag exists, check if item has it
	if (SlotTag.IsValid())
	{
		return Item->ItemTags.HasTag(SlotTag);
	}

	return true;
}
