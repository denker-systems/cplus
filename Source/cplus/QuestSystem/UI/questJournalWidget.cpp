// Fill out your copyright notice in the Description page of Project Settings.

#include "questJournalWidget.h"
#include "QuestSubSystem.h"
#include "Kismet/GameplayStatics.h"

void UQuestJournalWidget::RefreshJournal()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: RefreshJournal called"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: Calling Blueprint event OnQuestDataRefreshed"));
	OnQuestDataRefreshed();
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: OnQuestDataRefreshed complete"));
}

TArray<FQuestProgressData> UQuestJournalWidget::GetActiveQuests() const
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: GetActiveQuests called from Blueprint"));
	
	TArray<FQuestProgressData> Result;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST JOURNAL: GameInstance is NULL!"));
		return Result;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST JOURNAL: QuestSubsystem is NULL!"));
		return Result;
	}

	// Get active quests from subsystem
	TArray<FActiveQuest> ActiveQuests = QuestSubsystem->GetActiveQuests();
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: QuestSubsystem returned %d active quest(s)"), ActiveQuests.Num());
	
	for (const FActiveQuest& Quest : ActiveQuests)
	{
		if (Quest.QuestDefinition)
		{
			FQuestProgressData ProgressData;
			ProgressData.QuestID = Quest.QuestDefinition->QuestID;
			ProgressData.QuestState = static_cast<int32>(Quest.State);
			ProgressData.StartTime = Quest.AcceptedTime;
			
			// Build objective progress arrays
			for (int32 i = 0; i < Quest.QuestDefinition->Objectives.Num(); ++i)
			{
				bool bCompleted = (i < Quest.CurrentObjectiveIndex);
				ProgressData.ObjectiveCompleted.Add(bCompleted);
				ProgressData.ObjectiveProgress.Add(bCompleted ? 100 : 0);
			}
			
			Result.Add(ProgressData);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: Added quest to result: %s"), *ProgressData.QuestID.ToString());
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST JOURNAL: Returning %d quest(s) to Blueprint"), Result.Num());
	return Result;
}

TArray<FQuestProgressData> UQuestJournalWidget::GetCompletedQuests() const
{
	TArray<FQuestProgressData> Result;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return Result;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		return Result;
	}

	// Get completed quests from subsystem
	// Note: Currently QuestSubsystem doesn't track completed quests separately
	// This would need to be added to QuestSubsystem for full functionality
	
	return Result;
}

UQuestDefinition* UQuestJournalWidget::GetQuestDefinition(FName QuestID) const
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return nullptr;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		return nullptr;
	}

	// Get quest definition from subsystem
	return QuestSubsystem->GetQuestDefinition(QuestID);
}
