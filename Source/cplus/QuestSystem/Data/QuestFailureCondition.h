#pragma once

#include "CoreMinimal.h"
#include "QuestFailureCondition.generated.h"

UENUM(BlueprintType)
enum class EQuestFailureType : uint8
{
	TimeLimit UMETA(DisplayName = "Time Limit Exceeded"),
	NPCDied UMETA(DisplayName = "Important NPC Died"),
	ItemDestroyed UMETA(DisplayName = "Required Item Destroyed"),
	PlayerDied UMETA(DisplayName = "Player Died"),
	Custom UMETA(DisplayName = "Custom Condition")
};

/**
 * Quest Failure Condition
 * 
 * Defines conditions that cause quest to fail
 */
USTRUCT(BlueprintType)
struct FQuestFailureCondition
{
	GENERATED_BODY()

	/** Type of failure condition */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Failure")
	EQuestFailureType FailureType = EQuestFailureType::TimeLimit;

	/** Time limit in seconds (if FailureType = TimeLimit) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Failure", meta = (EditCondition = "FailureType == EQuestFailureType::TimeLimit"))
	float TimeLimit = 600.0f;

	/** NPC that must survive (if FailureType = NPCDied) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Failure", meta = (EditCondition = "FailureType == EQuestFailureType::NPCDied"))
	FName RequiredNPCID;

	/** Item that must not be destroyed (if FailureType = ItemDestroyed) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Failure", meta = (EditCondition = "FailureType == EQuestFailureType::ItemDestroyed"))
	FName RequiredItemID;
};
