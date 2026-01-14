// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubSystem.h"
#include "Core/PlayerProgressionComponent.h"
#include "InventorySystem/InventoryComponent.h"
#include "GameDataSubsystem.h"
#include "QuestSystem/Tasks/QuestTask_Collect.h"

void UQuestSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubSystem::ResetAllQuests()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: ResetAllQuests called"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Clearing %d active quest(s)"), ActiveQuests.Num());
	
	// Reset all task progress in active quests
	for (auto& Pair : ActiveQuests)
	{
		FActiveQuest& ActiveQuest = Pair.Value;
		if (ActiveQuest.QuestDefinition)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Resetting tasks for quest: %s"), *Pair.Key.ToString());
			
			// Reset all objectives
			for (FQuestObjective& Objective : ActiveQuest.QuestDefinition->Objectives)
			{
				// Reset all tasks in this objective
				for (UQuestTask* Task : Objective.Tasks)
				{
					if (Task)
					{
						Task->CurrentCount = 0;
						UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Reset task: %s (0/%d)"), 
							*Task->TaskDescription.ToString(), Task->TargetCount);
					}
				}
			}
		}
	}
	
	// Clear all active quests
	ActiveQuests.Empty();
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: All quests reset"));
}

bool UQuestSubSystem::RemoveQuest(FName QuestID)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: RemoveQuest called for %s"), *QuestID.ToString());
	
	if (ActiveQuests.Contains(QuestID))
	{
		ActiveQuests.Remove(QuestID);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Quest removed. ActiveQuests.Num() = %d"), ActiveQuests.Num());
		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Quest not found in active quests"));
	return false;
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
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: ProcessQuestEvent START for Quest=%s"), *QuestID.ToString());
	
	if (!ActiveQuest.QuestDefinition)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST SUBSYSTEM: QuestDefinition is NULL!"));
		return;
	}

	if (ActiveQuest.State != EQuestState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Quest is not active (State=%d)"), (int32)ActiveQuest.State);
		return;
	}

	if (ActiveQuest.CurrentObjectiveIndex >= ActiveQuest.QuestDefinition->Objectives.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Invalid objective index %d (Objectives.Num=%d)"), 
			ActiveQuest.CurrentObjectiveIndex, ActiveQuest.QuestDefinition->Objectives.Num());
		return;
	}

	FQuestObjective& CurrentObjective = ActiveQuest.QuestDefinition->Objectives[ActiveQuest.CurrentObjectiveIndex];
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Current objective has %d task(s)"), CurrentObjective.Tasks.Num());

	for (int32 TaskIndex = 0; TaskIndex < CurrentObjective.Tasks.Num(); ++TaskIndex)
	{
		UQuestTask* Task = CurrentObjective.Tasks[TaskIndex];
		if (!Task)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Task[%d] is NULL!"), TaskIndex);
			continue;
		}
		
		if (Task->IsComplete())
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task[%d] already complete, skipping"), TaskIndex);
			continue;
		}

		UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Checking Task[%d]: %s"), TaskIndex, *Task->TaskDescription.ToString());
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task progress: %d/%d"), Task->CurrentCount, Task->TargetCount);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task tags: %d, Event tags: %d"), Task->TaskTags.Num(), EventTags.Num());

		bool bTaskMatched = false;

		// Check if event tags match task tags
		if (Task->TaskTags.Num() > 0 && EventTags.Num() > 0 && Task->TaskTags.HasAny(EventTags))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task matched by TAGS!"));
			bTaskMatched = true;
		}
		// Special handling for Collect tasks - match by ItemID
		else if (UQuestTask_Collect* CollectTask = Cast<UQuestTask_Collect>(Task))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: This is a Collect task, ItemID=%s"), *CollectTask->ItemID.ToString());
			
			// For collect tasks, we need to check if any EventTag contains the ItemID
			// This is a workaround since collectibles send ItemID as part of the event
			// In the future, we should use a proper ItemCollected event with ItemID parameter
			
			// For now, just increment progress for ANY collect task when EventTags is empty
			// This means the collectible was picked up
			if (EventTags.Num() == 0)
			{
				UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Collect task matched (no tags = item collected)"));
				bTaskMatched = true;
			}
		}

		if (bTaskMatched)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task matched! Quest=%s, Task=%s"), 
				*QuestID.ToString(), *Task->TaskDescription.ToString());
			
			Task->UpdateProgress(1);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task progress updated: %d/%d"), 
				Task->CurrentCount, Task->TargetCount);
			
			OnQuestTaskUpdated.Broadcast(QuestID, ActiveQuest.CurrentObjectiveIndex, TaskIndex);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: OnQuestTaskUpdated broadcasted"));

			if (CurrentObjective.IsComplete())
			{
				UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Objective complete! Moving to next objective"));
				ActiveQuest.CurrentObjectiveIndex++;

				if (ActiveQuest.CurrentObjectiveIndex >= ActiveQuest.QuestDefinition->Objectives.Num())
				{
					UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Quest complete! %s"), *QuestID.ToString());
					ActiveQuest.State = EQuestState::Completed;
					
					// NOTE: Rewards are NOT given here - player must turn in quest to NPC
					UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Quest marked as Completed - return to NPC to turn in"));
					
					OnQuestCompleted.Broadcast(QuestID, ActiveQuest.QuestDefinition->Rewards);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Task[%d] did NOT match"), TaskIndex);
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: ProcessQuestEvent END"));
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
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST SUBSYSTEM: Quest is NULL!"));
		return;
	}

	// Check if quest is already active or completed
	if (ActiveQuests.Contains(Quest->QuestID))
	{
		FActiveQuest* ExistingQuest = ActiveQuests.Find(Quest->QuestID);
		if (ExistingQuest)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Quest %s is already active (State=%d)"), 
				*Quest->QuestID.ToString(), (int32)ExistingQuest->State);
			
			if (ExistingQuest->State == EQuestState::Completed)
			{
				UE_LOG(LogTemp, Warning, TEXT(">>> QUEST SUBSYSTEM: Quest is completed - player should turn it in first!"));
			}
			return;
		}
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

void UQuestSubSystem::LoadQuestProgress(FName QuestID, uint8 State, int32 CurrentObjectiveIndex, const TArray<int32>& TaskProgress, FDateTime AcceptedTime)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: LoadQuestProgress called"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: QuestID = %s, State = %d"), *QuestID.ToString(), State);

	// Find quest definition (need to load from GameDataSubsystem or AssetRegistry)
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST SUBSYSTEM: No GameInstance"));
		return;
	}

	UGameDataSubsystem* GameData = GameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!GameData)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST SUBSYSTEM: No GameDataSubsystem"));
		return;
	}

	// Get quest definition
	UQuestDefinition* QuestDef = GameData->GetQuestByID(QuestID);
	if (!QuestDef)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST SUBSYSTEM: Quest definition not found: %s"), *QuestID.ToString());
		return;
	}

	// Create active quest entry
	FActiveQuest NewActiveQuest;
	NewActiveQuest.QuestDefinition = QuestDef;
	NewActiveQuest.State = (EQuestState)State;
	NewActiveQuest.CurrentObjectiveIndex = CurrentObjectiveIndex;
	NewActiveQuest.AcceptedTime = AcceptedTime;

	// Restore task progress
	if (CurrentObjectiveIndex < QuestDef->Objectives.Num())
	{
		FQuestObjective& Objective = QuestDef->Objectives[CurrentObjectiveIndex];
		for (int32 i = 0; i < Objective.Tasks.Num() && i < TaskProgress.Num(); ++i)
		{
			if (UQuestTask* Task = Objective.Tasks[i])
			{
				Task->CurrentCount = TaskProgress[i];
				UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Restored task progress: %d/%d"),
					Task->CurrentCount, Task->TargetCount);
			}
		}
	}

	// Add to active quests
	ActiveQuests.Add(QuestID, NewActiveQuest);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST SUBSYSTEM: Quest progress loaded: %s"), *QuestID.ToString());
}

