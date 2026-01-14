// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestDefinition.h"
#include "GameplayTagContainer.h"
#include "QuestSubSystem.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NotStarted,
	Active,
	Completed,
	Failed
};

USTRUCT(BlueprintType)
struct FActiveQuest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UQuestDefinition> QuestDefinition;

	UPROPERTY(BlueprintReadOnly)
	EQuestState State = EQuestState::NotStarted;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentObjectiveIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	FDateTime AcceptedTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestStarted, FName, QuestID, UQuestDefinition*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted, FName, QuestID, const FQuestReward&, Rewards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestTaskUpdated, FName, QuestID, int32, ObjectiveIndex, int32, TaskIndex);

UCLASS()
class CPLUS_API UQuestSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestStarted OnQuestStarted;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestCompleted OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestTaskUpdated OnQuestTaskUpdated;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void NotifyQuestEvent(FName QuestID, const FGameplayTagContainer& EventTags, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateTaskProgress(FName QuestID, FName TaskID, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AcceptQuest(UQuestDefinition* Quest);

	/**
	 * Apply quest rewards to player
	 * @param PlayerActor The player to give rewards to
	 * @param Rewards The rewards to apply
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ApplyQuestRewards(AActor* PlayerActor, const FQuestReward& Rewards);

	/**
	 * Get all active quests
	 * @return Array of active quest data
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	TArray<FActiveQuest> GetActiveQuests() const;

	/**
	 * Get quest definition by ID
	 * @param QuestID The quest to retrieve
	 * @return Quest definition or nullptr
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	UQuestDefinition* GetQuestDefinition(FName QuestID) const;

	/**
	 * Check if quest is active
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	bool IsQuestActive(FName QuestID) const;

	FActiveQuest* FindActiveQuest(FName QuestID);

protected:
	/**
	 * Process quest event for a specific quest
	 * Helper method called by NotifyQuestEvent
	 */
	void ProcessQuestEvent(FName QuestID, FActiveQuest& ActiveQuest, const FGameplayTagContainer& EventTags, AActor* Instigator);

	UPROPERTY()
	TMap<FName, FActiveQuest> ActiveQuests;
};
