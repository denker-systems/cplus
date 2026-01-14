// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubSystem.h"
#include "Core/PlayerProgressionComponent.h"
#include "InventorySystem/InventoryComponent.h"
#include "GameDataSubsystem.h"

void UQuestSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubSystem::NotifyQuestEvent(FName QuestID, const FGameplayTagContainer& EventTags, AActor* Instigator)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: NotifyQuestEvent called (QuestID=%s, EventTags=%d)"), 
		*QuestID.ToString(), EventTags.Num());

	// If QuestID is NAME_None, check ALL active quests
	if (QuestID == NAME_None)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Checking all %d active quest(s)"), ActiveQuests.Num());
		
		for (auto& Pair : ActiveQuests)
		{
			ProcessQuestEvent(Pair.Key, Pair.Value, EventTags, Instigator);
		}
		return;
	}

	// Specific quest ID provided
	FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
	if (!ActiveQuest)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Quest not found: %s"), *QuestID.ToString());
		return;
	}

	ProcessQuestEvent(QuestID, *ActiveQuest, EventTags, Instigator);
}

void UQuestSubSystem::ProcessQuestEvent(FName QuestID, FActiveQuest& ActiveQuest, const FGameplayTagContainer& EventTags, AActor* Instigator)
{
	if (!ActiveQuest.QuestDefinition)
	{
		return;
	}

	if (ActiveQuest.State != EQuestState::Active)
	{
		return;
	}

	if (ActiveQuest.CurrentObjectiveIndex >= ActiveQuest.QuestDefinition->Objectives.Num())
	{
		return;
	}

	FQuestObjective& CurrentObjective = ActiveQuest.QuestDefinition->Objectives[ActiveQuest.CurrentObjectiveIndex];

	for (int32 TaskIndex = 0; TaskIndex < CurrentObjective.Tasks.Num(); ++TaskIndex)
	{
		UQuestTask* Task = CurrentObjective.Tasks[TaskIndex];
		if (!Task || Task->IsComplete())
		{
			continue;
		}

		// Check if event tags match task tags
		if (Task->TaskTags.HasAny(EventTags))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task matched! Quest=%s, Task=%s"), 
				*QuestID.ToString(), *Task->TaskDescription.ToString());
			
			Task->UpdateProgress(1);
			OnQuestTaskUpdated.Broadcast(QuestID, ActiveQuest.CurrentObjectiveIndex, TaskIndex);

			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task progress: %d/%d"), 
				Task->CurrentCount, Task->TargetCount);

			if (CurrentObjective.IsComplete())
			{
				UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Objective complete! Moving to next objective"));
				ActiveQuest.CurrentObjectiveIndex++;

				if (ActiveQuest.CurrentObjectiveIndex >= ActiveQuest.QuestDefinition->Objectives.Num())
				{
					UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Quest complete! %s"), *QuestID.ToString());
					ActiveQuest.State = EQuestState::Completed;
					
					// Apply rewards to player
					if (Instigator)
					{
						ApplyQuestRewards(Instigator, ActiveQuest.QuestDefinition->Rewards);
					}
					
					OnQuestCompleted.Broadcast(QuestID, ActiveQuest.QuestDefinition->Rewards);
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
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: AcceptQuest called"));
	
	if (!Quest)
	{
		return;
	}

	FActiveQuest NewQuest;
	NewQuest.QuestDefinition = Quest;
	NewQuest.State = EQuestState::Active;
	NewQuest.CurrentObjectiveIndex = 0;
	NewQuest.AcceptedTime = FDateTime::Now();

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Adding quest to ActiveQuests map"));
	ActiveQuests.Add(Quest->QuestID, NewQuest);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: ActiveQuests.Num() = %d"), ActiveQuests.Num());

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Broadcasting OnQuestStarted delegate"));
	OnQuestStarted.Broadcast(Quest->QuestID, Quest);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: OnQuestStarted broadcast complete"));
}

void UQuestSubSystem::ApplyQuestRewards(AActor* PlayerActor, const FQuestReward& Rewards)
{
	if (!PlayerActor)
	{
		return;
	}

	// Get PlayerProgressionComponent for XP, Gold, Reputation
	UPlayerProgressionComponent* ProgressionComp = PlayerActor->FindComponentByClass<UPlayerProgressionComponent>();
	if (ProgressionComp)
	{
		// Apply experience
		if (Rewards.ExperiencePoints > 0)
		{
			ProgressionComp->AddExperience(Rewards.ExperiencePoints);
			UE_LOG(LogTemp, Log, TEXT("Quest Reward: +%d XP"), Rewards.ExperiencePoints);
		}

		// Apply gold
		if (Rewards.Gold > 0)
		{
			ProgressionComp->AddGold(Rewards.Gold);
			UE_LOG(LogTemp, Log, TEXT("Quest Reward: +%d Gold"), Rewards.Gold);
		}

		// Apply reputation
		if (Rewards.ReputationGain > 0)
		{
			// TODO: Add faction system - for now just log
			UE_LOG(LogTemp, Log, TEXT("Quest Reward: +%d Reputation"), Rewards.ReputationGain);
		}
	}

	// Get InventoryComponent for item rewards
	UInventoryComponent* InventoryComp = PlayerActor->FindComponentByClass<UInventoryComponent>();
	if (InventoryComp && Rewards.ItemRewards.Num() > 0)
	{
		// Get GameDataSubsystem to load item definitions
		UGameDataSubsystem* GameData = GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
		if (GameData)
		{
			for (const FName& ItemID : Rewards.ItemRewards)
			{
				// Load item definition from GameDataSubsystem
				UItemDefinition* ItemDef = GameData->GetItemByID(ItemID);
				if (ItemDef)
				{
					int32 Added = InventoryComp->AddItem(ItemDef, 1);
					if (Added > 0)
					{
						UE_LOG(LogTemp, Log, TEXT("Quest Reward: +%s"), *ItemID.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Quest Reward: Item not found: %s"), *ItemID.ToString());
				}
			}
		}
	}

	// Apply custom rewards
	for (const auto& CustomReward : Rewards.CustomRewards)
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Reward: Custom %s = %d"), *CustomReward.Key.ToString(), CustomReward.Value);
		// Custom rewards can be handled by game-specific systems via delegates
	}
}

TArray<FActiveQuest> UQuestSubSystem::GetActiveQuests() const
{
	TArray<FActiveQuest> Result;
	for (const auto& Pair : ActiveQuests)
	{
		if (Pair.Value.State == EQuestState::Active)
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

UQuestDefinition* UQuestSubSystem::GetQuestDefinition(FName QuestID) const
{
	const FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
	if (ActiveQuest && ActiveQuest->QuestDefinition)
	{
		return ActiveQuest->QuestDefinition;
	}
	return nullptr;
}

bool UQuestSubSystem::IsQuestActive(FName QuestID) const
{
	const FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
	return ActiveQuest && ActiveQuest->State == EQuestState::Active;
}

FActiveQuest* UQuestSubSystem::FindActiveQuest(FName QuestID)
{
	return ActiveQuests.Find(QuestID);
}

