#include "QuestTestUtility.h"
#include "QuestDefinition.h"
#include "QuestGeneratorWidget.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Misc/Paths.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"

void UQuestTestUtility::CreateQuestGeneratorWidget()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: ========== Creating Quest Generator Widget =========="));

	// Create package
	FString PackageName = TEXT("/Game/Plugin/Quests/Utility/EUW_QuestGenerator");
	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UTILITY: Failed to create package"));
		return;
	}

	// Create Widget Blueprint
	UWidgetBlueprint* WidgetBP = NewObject<UWidgetBlueprint>(Package, TEXT("EUW_QuestGenerator"), RF_Public | RF_Standalone);
	if (!WidgetBP)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UTILITY: Failed to create Widget Blueprint"));
		return;
	}

	// Set parent class to QuestGeneratorWidget
	WidgetBP->ParentClass = UQuestGeneratorWidget::StaticClass();
	
	// Create widget tree
	WidgetBP->WidgetTree = NewObject<UWidgetTree>(WidgetBP, NAME_None, RF_Transactional);

	// Compile blueprint
	FKismetEditorUtilities::CompileBlueprint(WidgetBP);

	// Mark package dirty
	Package->MarkPackageDirty();

	// Save package
	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	
	if (UPackage::SavePackage(Package, WidgetBP, *PackageFileName, SaveArgs))
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: Widget Blueprint created successfully!"));
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: Location: %s"), *PackageName);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: Right-click in Content Browser -> Run Editor Utility Widget"));
		
		// Notify asset registry
		FAssetRegistryModule::AssetCreated(WidgetBP);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UTILITY: Failed to save Widget Blueprint"));
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: ========== Widget Creation Complete =========="));
}

void UQuestTestUtility::GenerateTestQuests(int32 NumQuests)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: ========== Generating %d Test Quests =========="), NumQuests);

	// Clamp to reasonable range
	NumQuests = FMath::Clamp(NumQuests, 1, 10);

	int32 SuccessCount = 0;
	for (int32 i = 0; i < NumQuests; i++)
	{
		UQuestDefinition* Quest = CreateTestQuest(i + 1);
		if (Quest)
		{
			SuccessCount++;
		}
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: ========== Generated %d/%d Test Quests Successfully =========="), 
		SuccessCount, NumQuests);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: Find them in Content Browser: /Game/Plugin/Quests/TestQuests/"));
}

UQuestDefinition* UQuestTestUtility::CreateTestQuest(int32 QuestIndex)
{
	// Define quest data based on index
	FString QuestIDStr = FString::Printf(TEXT("TestQuest_%d"), QuestIndex);
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
	FString PackageName = FString::Printf(TEXT("/Game/Plugin/Quests/TestQuests/DA_TestQuest_%d"), QuestIndex);
	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UTILITY: Failed to create package: %s"), *PackageName);
		return nullptr;
	}

	// Create quest definition asset
	FString AssetName = FString::Printf(TEXT("DA_TestQuest_%d"), QuestIndex);
	UQuestDefinition* QuestDef = NewObject<UQuestDefinition>(Package, *AssetName, RF_Public | RF_Standalone);
	if (!QuestDef)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UTILITY: Failed to create quest definition: %s"), *AssetName);
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
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UTILITY: Created quest: %s (%s)"), 
			*QuestTitle.ToString(), *QuestIDStr);
		
		// Notify asset registry
		FAssetRegistryModule::AssetCreated(QuestDef);
		
		return QuestDef;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UTILITY: Failed to save quest: %s"), *AssetName);
		return nullptr;
	}
}
