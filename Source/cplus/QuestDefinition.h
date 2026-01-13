// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "QuestTask.h"
#include "QuestDefinition.generated.h"

UENUM(BlueprintType)
enum class EQuestCompletionRule : uint8
{
	All UMETA(DisplayName = "Complete All Tasks"),
	Any UMETA(DisplayName = "Complete Any Task"),
	NOfM UMETA(DisplayName = "Complete N of M Tasks")
};

USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestTask>> Tasks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	EQuestCompletionRule CompletionRule = EQuestCompletionRule::All;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (EditCondition = "CompletionRule == EQuestCompletionRule::NOfM"))
	int32 RequiredTaskCount = 1;

	bool IsComplete() const;
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 ExperiencePoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FName> ItemRewards;
};

UCLASS(BlueprintType)
class CPLUS_API UQuestDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (MultiLine = true))
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FGameplayTagContainer QuestTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FQuestObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FQuestReward Rewards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FName> PrerequisiteQuests;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 MinimumLevel = 1;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
