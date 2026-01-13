#pragma once

#include "CoreMinimal.h"
#include "QuestMissionData.h"
#include "QuestCutsceneData.h"
#include "QuestStageData.generated.h"

/**
 * Quest Stage Data
 * 
 * Represents a phase/stage in a quest
 * Each stage contains multiple missions
 */
USTRUCT(BlueprintType)
struct FQuestStageData
{
	GENERATED_BODY()

	/** Stage identifier */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	FName StageID;

	/** Stage title (e.g. "Act 1: The Beginning") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	FText StageTitle;

	/** Missions in this stage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Missions")
	TArray<FQuestMissionData> Missions;

	/** Cutscenes for this stage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Cutscenes")
	TArray<FQuestCutsceneData> Cutscenes;

	/** Dialogs when stage starts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Dialogs")
	TArray<FQuestDialogData> StageStartDialogs;

	/** Dialogs when stage completes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Dialogs")
	TArray<FQuestDialogData> StageCompletionDialogs;
};
