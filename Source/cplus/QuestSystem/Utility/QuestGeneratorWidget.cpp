#include "QuestGeneratorWidget.h"
#include "QuestDefinition.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "UObject/SavePackage.h"
#include "Misc/Paths.h"

void UQuestGeneratorWidget::GenerateQuests()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: ========== Generating %d Quests =========="), NumQuests);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Prefix: %s"), *QuestPrefix);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Output: %s"), *OutputFolder);

	// Clear previous list
	GeneratedQuestNames.Empty();

	int32 SuccessCount = 0;
	for (int32 i = 0; i < NumQuests; i++)
	{
		UQuestDefinition* Quest = CreateTestQuest(i + 1);
		if (Quest)
		{
			SuccessCount++;
			GeneratedQuestNames.Add(Quest->QuestID.ToString());
		}
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: ========== Generated %d/%d Quests Successfully =========="), 
		SuccessCount, NumQuests);
	
	if (SuccessCount > 0)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Find them in Content Browser: %s"), *OutputFolder);
	}
}

void UQuestGeneratorWidget::ClearGeneratedQuests()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Clearing generated quest list"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Note: Assets must be deleted manually from Content Browser"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Location: %s"), *OutputFolder);
	
	GeneratedQuestNames.Empty();
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Quest list cleared"));
}

UQuestDefinition* UQuestGeneratorWidget::CreateTestQuest(int32 QuestIndex)
{
	// Define quest data based on index
	FString QuestIDStr = FString::Printf(TEXT("%s_%d"), *QuestPrefix, QuestIndex);
	FName QuestID = FName(*QuestIDStr);
	
	FText QuestTitle;
	FText ShortDesc;
	FText FullDesc;

	switch (QuestIndex)
	{
	case 1:
		QuestTitle = FText::FromString(TEXT("The First Step"));
		ShortDesc = FText::FromString(TEXT("Begin your journey by talking to the quest giver."));
		FullDesc = FText::FromString(TEXT("Welcome, adventurer! This is your first quest. Simply accept this quest to begin your journey. This is a test quest to verify the quest system works correctly."));
		break;
	case 2:
		QuestTitle = FText::FromString(TEXT("Exploration Time"));
		ShortDesc = FText::FromString(TEXT("Explore the area and return."));
		FullDesc = FText::FromString(TEXT("Now that you've started, it's time to explore! Walk around the area and familiarize yourself with the controls. Return when you're ready for your next quest."));
		break;
	case 3:
		QuestTitle = FText::FromString(TEXT("Testing the System"));
		ShortDesc = FText::FromString(TEXT("Verify all quest features work."));
		FullDesc = FText::FromString(TEXT("This quest tests the quest tracking system. Accept it, check your quest journal, and verify everything displays correctly. This helps ensure the quest system is functioning properly."));
		break;
	case 4:
		QuestTitle = FText::FromString(TEXT("Advanced Testing"));
		ShortDesc = FText::FromString(TEXT("Test quest completion and rewards."));
		FullDesc = FText::FromString(TEXT("An advanced test quest for verifying quest completion mechanics. This quest will help test the reward system and quest state transitions."));
		break;
	case 5:
		QuestTitle = FText::FromString(TEXT("Final Verification"));
		ShortDesc = FText::FromString(TEXT("Complete the final test."));
		FullDesc = FText::FromString(TEXT("The final test quest. Accept and complete this to verify the entire quest pipeline works from start to finish."));
		break;
	default:
		QuestTitle = FText::FromString(FString::Printf(TEXT("Test Quest %d"), QuestIndex));
		ShortDesc = FText::FromString(FString::Printf(TEXT("Test quest number %d."), QuestIndex));
		FullDesc = FText::FromString(FString::Printf(TEXT("This is test quest number %d. It's automatically generated for testing purposes."), QuestIndex));
		break;
	}

	// Create package path
	FString AssetName = FString::Printf(TEXT("DA_%s_%d"), *QuestPrefix, QuestIndex);
	FString PackageName = FString::Printf(TEXT("%s/%s"), *OutputFolder, *AssetName);
	
	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST GENERATOR: Failed to create package: %s"), *PackageName);
		return nullptr;
	}

	// Create quest definition asset
	UQuestDefinition* QuestDef = NewObject<UQuestDefinition>(Package, *AssetName, RF_Public | RF_Standalone);
	if (!QuestDef)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST GENERATOR: Failed to create quest definition: %s"), *AssetName);
		return nullptr;
	}

	// Set quest data
	QuestDef->QuestID = QuestID;
	QuestDef->UIData.QuestTitle = QuestTitle;
	QuestDef->UIData.ShortDescription = ShortDesc;
	QuestDef->UIData.FullDescription = FullDesc;
	QuestDef->UIData.QuestColor = FLinearColor(1.0f, 0.8f, 0.2f); // Gold color

	// Mark package as dirty
	Package->MarkPackageDirty();

	// Save package
	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	
	if (UPackage::SavePackage(Package, QuestDef, *PackageFileName, SaveArgs))
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Created quest: %s (%s)"), 
			*QuestTitle.ToString(), *QuestIDStr);
		
		// Notify asset registry
		FAssetRegistryModule::AssetCreated(QuestDef);
		
		return QuestDef;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST GENERATOR: Failed to save quest: %s"), *AssetName);
		return nullptr;
	}
}
