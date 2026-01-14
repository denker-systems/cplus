#include "QuestGeneratorWidget.h"
#include "QuestDefinition.h"
#include "QuestTask_Kill.h"
#include "QuestTask_Collect.h"
#include "QuestTask_Interact.h"
#include "QuestTask_Location.h"
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

	// Set quest settings
	QuestDef->bCanAbandon = true;
	QuestDef->bCanFail = false;
	QuestDef->bAutoTrack = true;
	QuestDef->MinimumLevel = FMath::Max(1, QuestIndex - 1);

	// Generate objectives
	if (bGenerateObjectives)
	{
		FQuestObjective Objective;
		Objective.ObjectiveDescription = FText::FromString(FString::Printf(TEXT("Complete objective for %s"), *QuestTitle.ToString()));
		Objective.bIsOptional = false;
		Objective.bIsHidden = false;
		Objective.DisplayOrder = 0;

		// Create task based on quest index
		switch (QuestIndex % 4)
		{
		case 0: // Kill task
			{
				UQuestTask_Kill* KillTask = NewObject<UQuestTask_Kill>(QuestDef, UQuestTask_Kill::StaticClass());
				KillTask->TaskID = FName(*FString::Printf(TEXT("%s_Kill"), *QuestIDStr));
				KillTask->TaskDescription = FText::FromString(TEXT("Defeat enemies"));
				KillTask->TargetCount = 5 + (QuestIndex * 2);
				KillTask->CurrentCount = 0;
				KillTask->EnemyTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Enemy.Hostile")));
				Objective.Tasks.Add(KillTask);
				Objective.ObjectiveDescription = FText::FromString(FString::Printf(TEXT("Defeat %d enemies"), KillTask->TargetCount));
			}
			break;

		case 1: // Collect task
			{
				UQuestTask_Collect* CollectTask = NewObject<UQuestTask_Collect>(QuestDef, UQuestTask_Collect::StaticClass());
				CollectTask->TaskID = FName(*FString::Printf(TEXT("%s_Collect"), *QuestIDStr));
				CollectTask->TaskDescription = FText::FromString(TEXT("Collect items"));
				CollectTask->TargetCount = 3 + QuestIndex;
				CollectTask->CurrentCount = 0;
				CollectTask->ItemID = FName(TEXT("Item.QuestItem"));
				Objective.Tasks.Add(CollectTask);
				Objective.ObjectiveDescription = FText::FromString(FString::Printf(TEXT("Collect %d items"), CollectTask->TargetCount));
			}
			break;

		case 2: // Interact task
			{
				UQuestTask_Interact* InteractTask = NewObject<UQuestTask_Interact>(QuestDef, UQuestTask_Interact::StaticClass());
				InteractTask->TaskID = FName(*FString::Printf(TEXT("%s_Interact"), *QuestIDStr));
				InteractTask->TaskDescription = FText::FromString(TEXT("Interact with objects"));
				InteractTask->TargetCount = 1;
				InteractTask->CurrentCount = 0;
				Objective.Tasks.Add(InteractTask);
				Objective.ObjectiveDescription = FText::FromString(TEXT("Interact with the quest object"));
			}
			break;

		case 3: // Location task
			{
				UQuestTask_Location* LocationTask = NewObject<UQuestTask_Location>(QuestDef, UQuestTask_Location::StaticClass());
				LocationTask->TaskID = FName(*FString::Printf(TEXT("%s_Location"), *QuestIDStr));
				LocationTask->TaskDescription = FText::FromString(TEXT("Reach the destination"));
				LocationTask->TargetCount = 1;
				LocationTask->CurrentCount = 0;
				LocationTask->TargetLocation = FVector(1000.0f * QuestIndex, 0.0f, 100.0f);
				LocationTask->AcceptanceRadius = 500.0f;
				Objective.Tasks.Add(LocationTask);
				Objective.ObjectiveDescription = FText::FromString(TEXT("Travel to the marked location"));
			}
			break;
		}

		QuestDef->Objectives.Add(Objective);
	}

	// Generate rewards
	if (bGenerateRewards)
	{
		QuestDef->Rewards.ExperiencePoints = FMath::RandRange(MinXPReward, MaxXPReward);
		QuestDef->Rewards.Gold = FMath::RandRange(MinGoldReward, MaxGoldReward);
		QuestDef->Rewards.ReputationGain = FMath::RandRange(5, 20);

		// Add item rewards for some quests
		if (QuestIndex % 2 == 0)
		{
			QuestDef->Rewards.ItemRewards.Add(FName(TEXT("Item.Reward.Potion")));
		}
		if (QuestIndex % 3 == 0)
		{
			QuestDef->Rewards.ItemRewards.Add(FName(TEXT("Item.Reward.Gold")));
		}
	}

	// Generate dialogs
	if (bGenerateDialogs)
	{
		// Intro dialog
		FQuestDialogData IntroDialog;
		IntroDialog.SpeakerID = FName(TEXT("QuestGiver"));
		IntroDialog.SpeakerName = FText::FromString(TEXT("Quest Giver"));
		IntroDialog.DialogText = FText::FromString(FString::Printf(
			TEXT("Greetings, adventurer! I have a task for you. %s Are you willing to help?"),
			*ShortDesc.ToString()));
		QuestDef->IntroDialogs.Add(IntroDialog);

		// Completion dialog
		FQuestDialogData CompletionDialog;
		CompletionDialog.SpeakerID = FName(TEXT("QuestGiver"));
		CompletionDialog.SpeakerName = FText::FromString(TEXT("Quest Giver"));
		CompletionDialog.DialogText = FText::FromString(FString::Printf(
			TEXT("Excellent work! You have completed '%s'. Here is your reward."),
			*QuestTitle.ToString()));
		QuestDef->CompletionDialogs.Add(CompletionDialog);
	}

	// Mark package as dirty
	Package->MarkPackageDirty();

	// Save package
	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	
	// Ensure directory exists
	FString PackageDirectory = FPaths::GetPath(PackageFileName);
	if (!FPaths::DirectoryExists(PackageDirectory))
	{
		IFileManager::Get().MakeDirectory(*PackageDirectory, true);
		UE_LOG(LogTemp, Log, TEXT(">>> QUEST GENERATOR: Created directory: %s"), *PackageDirectory);
	}
	
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	
	UE_LOG(LogTemp, Log, TEXT(">>> QUEST GENERATOR: Attempting to save to: %s"), *PackageFileName);
	
	if (UPackage::SavePackage(Package, QuestDef, *PackageFileName, SaveArgs))
	{
		FString RewardInfo = FString::Printf(TEXT("XP:%d Gold:%d"), 
			QuestDef->Rewards.ExperiencePoints, 
			QuestDef->Rewards.Gold);
		FString ObjectiveInfo = QuestDef->Objectives.Num() > 0 ? 
			QuestDef->Objectives[0].ObjectiveDescription.ToString() : 
			TEXT("No objectives");
		
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST GENERATOR: Created quest: %s (%s)"), 
			*QuestTitle.ToString(), *QuestIDStr);
		UE_LOG(LogTemp, Display, TEXT(">>>   Objective: %s"), *ObjectiveInfo);
		UE_LOG(LogTemp, Display, TEXT(">>>   Rewards: %s"), *RewardInfo);
		
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
