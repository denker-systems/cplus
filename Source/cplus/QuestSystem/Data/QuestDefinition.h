// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include "QuestTask.h"
#include "QuestReward.h"
#include "questObjective.h"
#include "questUIData.h"
#include "QuestDialogData.h"
#include "QuestCutsceneData.h"
#include "QuestMissionData.h"
#include "QuestStageData.h"
#include "QuestFailureCondition.h"
#include "QuestDefinition.generated.h"

UENUM(BlueprintType)
enum class EQuestCompletionRule : uint8
{
	All UMETA(DisplayName = "Complete All Tasks"),
	Any UMETA(DisplayName = "Complete Any Task"),
	NOfM UMETA(DisplayName = "Complete N of M Tasks")
};

/**
 * Quest Definition
 * 
 * Data asset that defines a complete quest.
 * Uses data-driven design with separate structs for UI, objectives, and rewards.
 */
UCLASS(BlueprintType)
class CPLUS_API UQuestDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Unique identifier for this quest */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Identity")
	FName QuestID;

	/** UI data for this quest (title, description, icon, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|UI")
	FQuestUIData UIData;

	/** Quest objectives that must be completed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Objectives")
	TArray<FQuestObjective> Objectives;

	/** Rewards given upon quest completion */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Rewards")
	FQuestReward Rewards;

	/** Quest IDs that must be completed before this quest becomes available */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Requirements")
	TArray<FName> PrerequisiteQuests;

	/** Minimum player level required to accept this quest */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Requirements")
	int32 MinimumLevel = 1;

	/** Can this quest be abandoned once started? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Settings")
	bool bCanAbandon = true;

	/** Can this quest fail? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Settings")
	bool bCanFail = false;

	/** Is this quest automatically tracked when accepted? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Settings")
	bool bAutoTrack = true;

	// ===== QUEST GIVER =====

	/** NPC that gives this quest */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|NPC")
	FName QuestGiverID;

	// ===== STAGES & MISSIONS =====

	/** Quest stages (phases) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Stages")
	TArray<FQuestStageData> Stages;

	// ===== DIALOGS =====

	/** Dialogs when quest is offered */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Dialogs")
	TArray<FQuestDialogData> IntroDialogs;

	/** Dialogs when quest is completed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Dialogs")
	TArray<FQuestDialogData> CompletionDialogs;

	/** Dialogs when quest fails */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Dialogs")
	TArray<FQuestDialogData> FailureDialogs;

	// ===== CUTSCENES =====

	/** Cutscenes for this quest */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Cutscenes")
	TArray<FQuestCutsceneData> Cutscenes;

	// ===== AUDIO =====

	/** Background music during quest */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Audio")
	TObjectPtr<USoundBase> QuestMusic;

	/** Sound played when quest starts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Audio")
	TObjectPtr<USoundBase> QuestStartSound;

	/** Sound played when quest completes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Audio")
	TObjectPtr<USoundBase> QuestCompleteSound;

	// ===== FAILURE CONDITIONS =====

	/** Conditions that cause quest to fail */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Failure")
	TArray<FQuestFailureCondition> FailureConditions;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
