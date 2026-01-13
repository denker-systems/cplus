// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "questProgressData.generated.h"

/**
 * Quest Progress Data
 * 
 * Runtime data for tracking quest progress.
 * Used for save/load system and UI updates.
 * Serializable for persistent storage.
 */
USTRUCT(BlueprintType)
struct CPLUS_API FQuestProgressData
{
	GENERATED_BODY()

	/** Quest ID being tracked */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	FName QuestID;

	/** Current state of the quest (0=NotStarted, 1=Active, 2=Completed, 3=Failed) */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	int32 QuestState = 0;

	/** Progress per objective (index matches objective array in QuestDefinition) */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	TArray<int32> ObjectiveProgress;

	/** Completion status per objective */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	TArray<bool> ObjectiveCompleted;

	/** Timestamp when quest was started */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	FDateTime StartTime;

	/** Timestamp when quest was completed (or failed) */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	FDateTime CompletionTime;

	/** Has the player seen the completion notification? */
	UPROPERTY(BlueprintReadOnly, Category = "Progress")
	bool bNotificationShown = false;
};
