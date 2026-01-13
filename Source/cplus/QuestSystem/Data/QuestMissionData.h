#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "QuestDialogData.h"
#include "QuestMissionData.generated.h"

/**
 * Quest Mission Data
 * 
 * Represents a single mission within a quest stage
 * (e.g. "Mission 1: Gather supplies")
 */
USTRUCT(BlueprintType)
struct FQuestMissionData
{
	GENERATED_BODY()

	/** Mission title */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	FText MissionTitle;

	/** Mission description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission", meta = (MultiLine = true))
	FText MissionDescription;

	/** Tasks that must be completed for this mission */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Mission|Tasks")
	TArray<TObjectPtr<UQuestTask>> Tasks;

	/** Dialogs to play when mission starts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Dialogs")
	TArray<FQuestDialogData> StartDialogs;

	/** Dialogs to play when mission completes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Dialogs")
	TArray<FQuestDialogData> CompletionDialogs;

	/** Tracked locations for this mission (waypoints, markers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Tracking")
	TArray<FVector> TrackedLocations;
};
