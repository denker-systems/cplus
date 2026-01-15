#include "ItemGeneratorWidget.h"
#include "WorldItem.h"
#include "PickupComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Misc/Paths.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Factories/BlueprintFactory.h"
#include "KismetCompilerModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Engine/Blueprint.h"

void UItemGeneratorWidget::GenerateItems()
{
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: ========== Generating Items =========="));
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Items per type: %d"), ItemsPerType);
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Output: %s"), *OutputFolder);

	GeneratedItemNames.Empty();
	int32 SuccessCount = 0;

	// Generate each type
	if (bGenerateConsumables)
	{
		for (int32 i = 0; i < ItemsPerType; i++)
		{
			if (CreateItem(EItemType::Consumable, i + 1)) SuccessCount++;
		}
	}

	if (bGenerateWeapons)
	{
		for (int32 i = 0; i < ItemsPerType; i++)
		{
			if (CreateItem(EItemType::Weapon, i + 1)) SuccessCount++;
		}
	}

	if (bGenerateAmmo)
	{
		for (int32 i = 0; i < ItemsPerType; i++)
		{
			if (CreateItem(EItemType::Ammo, i + 1)) SuccessCount++;
		}
	}

	if (bGenerateQuestItems)
	{
		for (int32 i = 0; i < ItemsPerType; i++)
		{
			if (CreateItem(EItemType::QuestItem, i + 1)) SuccessCount++;
		}
	}

	if (bGenerateMaterials)
	{
		for (int32 i = 0; i < ItemsPerType; i++)
		{
			if (CreateItem(EItemType::Material, i + 1)) SuccessCount++;
		}
	}

	if (bGenerateMisc)
	{
		for (int32 i = 0; i < ItemsPerType; i++)
		{
			if (CreateItem(EItemType::Misc, i + 1)) SuccessCount++;
		}
	}

	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: ========== Generated %d Items =========="), SuccessCount);
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Find them in: %s"), *OutputFolder);
}

void UItemGeneratorWidget::ClearGeneratedItems()
{
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Clearing generated item list"));
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Note: Assets must be deleted manually from Content Browser"));
	GeneratedItemNames.Empty();
}

UItemDefinition* UItemGeneratorWidget::CreateItem(EItemType Type, int32 Index)
{
	// Build item data based on type
	FString TypeName;
	FString ItemName;
	FString ItemDesc;
	int32 MaxStack = 1;
	float Weight = 1.0f;
	int32 SellValue = 10;
	EItemRarity Rarity = EItemRarity::Common;
	bool bIsUsable = false;
	bool bIsQuestItem = false;
	float HealthRestore = 0.0f;

	switch (Type)
	{
	case EItemType::Consumable:
		TypeName = TEXT("Consumable");
		bIsUsable = true;
		MaxStack = 10;
		Weight = 0.5f;
		switch (Index)
		{
		case 1:
			ItemName = TEXT("Health Potion");
			ItemDesc = TEXT("Restores 50 health when used.");
			HealthRestore = 50.0f;
			SellValue = 25;
			Rarity = EItemRarity::Common;
			break;
		case 2:
			ItemName = TEXT("Greater Health Potion");
			ItemDesc = TEXT("Restores 100 health when used.");
			HealthRestore = 100.0f;
			SellValue = 75;
			Rarity = EItemRarity::Uncommon;
			break;
		default:
			ItemName = FString::Printf(TEXT("Potion %d"), Index);
			ItemDesc = FString::Printf(TEXT("A mysterious potion. Restores %d health."), Index * 25);
			HealthRestore = Index * 25.0f;
			SellValue = Index * 20;
			break;
		}
		break;

	case EItemType::Weapon:
		TypeName = TEXT("Weapon");
		MaxStack = 1;
		Weight = 5.0f;
		switch (Index)
		{
		case 1:
			ItemName = TEXT("Iron Sword");
			ItemDesc = TEXT("A basic iron sword.");
			SellValue = 50;
			Rarity = EItemRarity::Common;
			break;
		case 2:
			ItemName = TEXT("Steel Blade");
			ItemDesc = TEXT("A well-crafted steel blade.");
			SellValue = 150;
			Rarity = EItemRarity::Uncommon;
			break;
		default:
			ItemName = FString::Printf(TEXT("Weapon %d"), Index);
			ItemDesc = TEXT("A weapon for combat.");
			SellValue = Index * 50;
			break;
		}
		break;

	case EItemType::Ammo:
		TypeName = TEXT("Ammo");
		MaxStack = 99;
		Weight = 0.1f;
		switch (Index)
		{
		case 1:
			ItemName = TEXT("Pistol Ammo");
			ItemDesc = TEXT("Standard pistol ammunition.");
			SellValue = 1;
			break;
		case 2:
			ItemName = TEXT("Rifle Ammo");
			ItemDesc = TEXT("Standard rifle ammunition.");
			SellValue = 2;
			break;
		default:
			ItemName = FString::Printf(TEXT("Ammo Type %d"), Index);
			ItemDesc = TEXT("Ammunition for firearms.");
			SellValue = Index;
			break;
		}
		break;

	case EItemType::QuestItem:
		TypeName = TEXT("Quest");
		bIsQuestItem = true;
		MaxStack = 1;
		Weight = 0.0f;
		SellValue = 0;
		switch (Index)
		{
		case 1:
			ItemName = TEXT("Ancient Scroll");
			ItemDesc = TEXT("A mysterious scroll needed for a quest.");
			Rarity = EItemRarity::Rare;
			break;
		case 2:
			ItemName = TEXT("Strange Key");
			ItemDesc = TEXT("An ornate key. Someone might need this.");
			Rarity = EItemRarity::Uncommon;
			break;
		default:
			ItemName = FString::Printf(TEXT("Quest Item %d"), Index);
			ItemDesc = TEXT("An item needed for a quest.");
			break;
		}
		break;

	case EItemType::Material:
		TypeName = TEXT("Material");
		MaxStack = 50;
		Weight = 0.2f;
		switch (Index)
		{
		case 1:
			ItemName = TEXT("Iron Ore");
			ItemDesc = TEXT("Raw iron ore. Can be smelted.");
			SellValue = 5;
			break;
		case 2:
			ItemName = TEXT("Wood");
			ItemDesc = TEXT("A piece of wood. Useful for crafting.");
			SellValue = 2;
			break;
		default:
			ItemName = FString::Printf(TEXT("Material %d"), Index);
			ItemDesc = TEXT("A crafting material.");
			SellValue = Index * 3;
			break;
		}
		break;

	case EItemType::Misc:
	default:
		TypeName = TEXT("Misc");
		MaxStack = 20;
		Weight = 0.5f;
		switch (Index)
		{
		case 1:
			ItemName = TEXT("Gold Coin");
			ItemDesc = TEXT("A shiny gold coin.");
			SellValue = 1;
			MaxStack = 9999;
			Weight = 0.0f;
			break;
		case 2:
			ItemName = TEXT("Gem");
			ItemDesc = TEXT("A valuable gemstone.");
			SellValue = 100;
			Rarity = EItemRarity::Rare;
			break;
		default:
			ItemName = FString::Printf(TEXT("Misc Item %d"), Index);
			ItemDesc = TEXT("A miscellaneous item.");
			SellValue = Index * 5;
			break;
		}
		break;
	}

	// Create ItemID
	FString ItemIDStr = FString::Printf(TEXT("Item.%s.%s_%d"), *TypeName, *ItemPrefix, Index);
	FName ItemID = FName(*ItemIDStr);

	// Create package
	FString AssetName = FString::Printf(TEXT("DA_%s_%s_%d"), *ItemPrefix, *TypeName, Index);
	FString PackageName = FString::Printf(TEXT("%s/%s"), *OutputFolder, *AssetName);

	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> ITEM GENERATOR: Failed to create package: %s"), *PackageName);
		return nullptr;
	}

	// Create item definition
	UItemDefinition* ItemDef = NewObject<UItemDefinition>(Package, *AssetName, RF_Public | RF_Standalone);
	if (!ItemDef)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> ITEM GENERATOR: Failed to create item: %s"), *AssetName);
		return nullptr;
	}

	// Set item data
	ItemDef->ItemID = ItemID;
	ItemDef->ItemType = Type;
	ItemDef->ItemRarity = Rarity;
	ItemDef->ItemName = FText::FromString(ItemName);
	ItemDef->ItemDescription = FText::FromString(ItemDesc);
	ItemDef->RarityColor = GetRarityColor(Rarity);
	ItemDef->MaxStackSize = MaxStack;
	ItemDef->ItemWeight = Weight;
	ItemDef->SellValue = SellValue;
	ItemDef->bIsUsable = bIsUsable;
	ItemDef->bIsQuestItem = bIsQuestItem;
	ItemDef->HealthRestore = HealthRestore;
	ItemDef->bCanBeDropped = !bIsQuestItem;
	ItemDef->bCanBeSold = !bIsQuestItem;
	ItemDef->bCanBeDestroyed = !bIsQuestItem;

	// Set GameplayTags based on type
	switch (Type)
	{
	case EItemType::Consumable:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Consumable"))));
		if (HealthRestore > 0)
		{
			ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Consumable.Health"))));
		}
		break;
	case EItemType::Weapon:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Weapon"))));
		break;
	case EItemType::Ammo:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Ammo"))));
		break;
	case EItemType::QuestItem:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Quest"))));
		break;
	case EItemType::Material:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Craft"))));
		break;
	case EItemType::Misc:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Misc"))));
		break;
	default:
		break;
	}

	// Add rarity tag
	switch (Rarity)
	{
	case EItemRarity::Common:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Rarity.Common"))));
		break;
	case EItemRarity::Uncommon:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Rarity.Uncommon"))));
		break;
	case EItemRarity::Rare:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Rarity.Rare"))));
		break;
	case EItemRarity::Epic:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Rarity.Epic"))));
		break;
	case EItemRarity::Legendary:
		ItemDef->ItemTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Rarity.Legendary"))));
		break;
	}

	// Mark and save
	Package->MarkPackageDirty();

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	FString PackageDirectory = FPaths::GetPath(PackageFileName);
	if (!FPaths::DirectoryExists(PackageDirectory))
	{
		IFileManager::Get().MakeDirectory(*PackageDirectory, true);
	}

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;

	if (UPackage::SavePackage(Package, ItemDef, *PackageFileName, SaveArgs))
	{
		UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Created: %s [%s] Stack:%d Value:%d"),
			*ItemName, *TypeName, MaxStack, SellValue);

		FAssetRegistryModule::AssetCreated(ItemDef);
		GeneratedItemNames.Add(ItemID.ToString());

		return ItemDef;
	}

	UE_LOG(LogTemp, Error, TEXT(">>> ITEM GENERATOR: Failed to save: %s"), *AssetName);
	return nullptr;
}

FLinearColor UItemGeneratorWidget::GetRarityColor(EItemRarity Rarity) const
{
	switch (Rarity)
	{
	case EItemRarity::Common:    return FLinearColor(0.8f, 0.8f, 0.8f);  // Gray
	case EItemRarity::Uncommon:  return FLinearColor(0.2f, 0.8f, 0.2f);  // Green
	case EItemRarity::Rare:      return FLinearColor(0.2f, 0.4f, 1.0f);  // Blue
	case EItemRarity::Epic:      return FLinearColor(0.6f, 0.2f, 0.8f);  // Purple
	case EItemRarity::Legendary: return FLinearColor(1.0f, 0.6f, 0.0f);  // Orange
	default:                     return FLinearColor::White;
	}
}

void UItemGeneratorWidget::GenerateWorldItemBlueprints()
{
	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: ========== Generating WorldItem Blueprints =========="));

	if (!WorldItemClass)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> ITEM GENERATOR: WorldItemClass not set! Set it to AWorldItem or BP_WorldItem"));
		return;
	}

	// Find all ItemDefinition assets in output folder
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssetsByPath(FName(*OutputFolder), AssetDataList, true);

	int32 SuccessCount = 0;

	for (const FAssetData& AssetData : AssetDataList)
	{
		// Only process ItemDefinition assets
		if (AssetData.AssetClassPath.GetAssetName() != TEXT("ItemDefinition"))
		{
			continue;
		}

		UItemDefinition* ItemDef = Cast<UItemDefinition>(AssetData.GetAsset());
		if (!ItemDef)
		{
			continue;
		}

		// Create blueprint name from item
		FString ItemName = ItemDef->ItemName.ToString().Replace(TEXT(" "), TEXT(""));
		FString BPName = FString::Printf(TEXT("BP_%s"), *ItemName);
		FString BPPackageName = FString::Printf(TEXT("%s/%s"), *BlueprintOutputFolder, *BPName);

		// Check if already exists
		FAssetData ExistingAsset = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(BPPackageName + TEXT(".") + BPName));
		if (ExistingAsset.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> ITEM GENERATOR: Blueprint already exists: %s"), *BPName);
			continue;
		}

		// Create package
		UPackage* Package = CreatePackage(*BPPackageName);
		if (!Package)
		{
			UE_LOG(LogTemp, Error, TEXT(">>> ITEM GENERATOR: Failed to create package: %s"), *BPPackageName);
			continue;
		}

		// Create blueprint
		UBlueprint* NewBP = FKismetEditorUtilities::CreateBlueprint(
			WorldItemClass,
			Package,
			FName(*BPName),
			BPTYPE_Normal,
			UBlueprint::StaticClass(),
			UBlueprintGeneratedClass::StaticClass()
		);

		if (!NewBP)
		{
			UE_LOG(LogTemp, Error, TEXT(">>> ITEM GENERATOR: Failed to create blueprint: %s"), *BPName);
			continue;
		}

		// Get CDO and set ItemData on PickupComponent
		AActor* CDO = Cast<AActor>(NewBP->GeneratedClass->GetDefaultObject());
		if (CDO)
		{
			UPickupComponent* PickupComp = CDO->FindComponentByClass<UPickupComponent>();
			if (PickupComp)
			{
				PickupComp->ItemData = ItemDef;
				UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Set ItemData on %s"), *BPName);
			}
		}

		// Compile and save
		FKismetEditorUtilities::CompileBlueprint(NewBP);
		Package->MarkPackageDirty();

		FString PackageFileName = FPackageName::LongPackageNameToFilename(BPPackageName, FPackageName::GetAssetPackageExtension());

		FString PackageDirectory = FPaths::GetPath(PackageFileName);
		if (!FPaths::DirectoryExists(PackageDirectory))
		{
			IFileManager::Get().MakeDirectory(*PackageDirectory, true);
		}

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;

		if (UPackage::SavePackage(Package, NewBP, *PackageFileName, SaveArgs))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: Created BP: %s -> %s"),
				*BPName, *ItemDef->ItemName.ToString());
			FAssetRegistryModule::AssetCreated(NewBP);
			SuccessCount++;
		}
	}

	UE_LOG(LogTemp, Display, TEXT(">>> ITEM GENERATOR: ========== Generated %d Blueprints =========="), SuccessCount);
}
