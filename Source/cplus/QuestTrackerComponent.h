// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestDefinition.h"
#include "QuestSubSystem.h"
#include "QuestTrackerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestAccepted, FName, QuestID, UQuestDefinition*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestProgressed, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompletedTracker, FName, QuestID, const FQuestReward&, Rewards);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPLUS_API UQuestTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UQuestTrackerComponent();

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestAccepted OnQuestAccepted;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestProgressed OnQuestProgressed;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestCompletedTracker OnQuestCompleted;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AcceptQuest(UQuestDefinition* Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AbandonQuest(FName QuestID);

	UFUNCTION(BlueprintPure, Category = "Quest")
	TArray<FName> GetActiveQuestIDs() const;

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FName> ActiveQuestIDs;

	UPROPERTY()
	TArray<FName> CompletedQuestIDs;

	UQuestSubSystem* GetQuestSubsystem() const;
};
