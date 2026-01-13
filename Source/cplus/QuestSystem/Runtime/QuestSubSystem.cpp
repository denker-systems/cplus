// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubSystem.h"

void UQuestSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubSystem::NotifyQuestEvent(FName QuestID, const FGameplayTagContainer& EventTags, AActor* Instigator)
{
	FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
	if (!ActiveQuest || !ActiveQuest->QuestDefinition)
	{
		return;
	}

	if (ActiveQuest->CurrentObjectiveIndex >= ActiveQuest->QuestDefinition->Objectives.Num())
	{
		return;
	}

	FQuestObjective& CurrentObjective = ActiveQuest->QuestDefinition->Objectives[ActiveQuest->CurrentObjectiveIndex];

	for (int32 TaskIndex = 0; TaskIndex < CurrentObjective.Tasks.Num(); ++TaskIndex)
	{
		UQuestTask* Task = CurrentObjective.Tasks[TaskIndex];
		if (!Task || Task->IsComplete())
		{
			continue;
		}

		if (Task->TaskTags.HasAny(EventTags))
		{
			Task->UpdateProgress(1);
			OnQuestTaskUpdated.Broadcast(QuestID, ActiveQuest->CurrentObjectiveIndex, TaskIndex);

			if (CurrentObjective.IsComplete())
			{
				ActiveQuest->CurrentObjectiveIndex++;

				if (ActiveQuest->CurrentObjectiveIndex >= ActiveQuest->QuestDefinition->Objectives.Num())
				{
					ActiveQuest->State = EQuestState::Completed;
					OnQuestCompleted.Broadcast(QuestID, ActiveQuest->QuestDefinition->Rewards);
				}
			}
		}
	}
}

void UQuestSubSystem::UpdateTaskProgress(FName QuestID, FName TaskID, int32 Amount)
{
	FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
	if (!ActiveQuest || !ActiveQuest->QuestDefinition)
	{
		return;
	}

	if (ActiveQuest->CurrentObjectiveIndex >= ActiveQuest->QuestDefinition->Objectives.Num())
	{
		return;
	}

	FQuestObjective& CurrentObjective = ActiveQuest->QuestDefinition->Objectives[ActiveQuest->CurrentObjectiveIndex];

	for (int32 TaskIndex = 0; TaskIndex < CurrentObjective.Tasks.Num(); ++TaskIndex)
	{
		UQuestTask* Task = CurrentObjective.Tasks[TaskIndex];
		if (Task && Task->TaskID == TaskID)
		{
			Task->UpdateProgress(Amount);
			OnQuestTaskUpdated.Broadcast(QuestID, ActiveQuest->CurrentObjectiveIndex, TaskIndex);
			break;
		}
	}
}

void UQuestSubSystem::AcceptQuest(UQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}

	FActiveQuest NewQuest;
	NewQuest.QuestDefinition = Quest;
	NewQuest.State = EQuestState::Active;
	NewQuest.CurrentObjectiveIndex = 0;
	NewQuest.AcceptedTime = FDateTime::Now();

	ActiveQuests.Add(Quest->QuestID, NewQuest);
	OnQuestStarted.Broadcast(Quest->QuestID, Quest);
}

FActiveQuest* UQuestSubSystem::FindActiveQuest(FName QuestID)
{
	return ActiveQuests.Find(QuestID);
}

