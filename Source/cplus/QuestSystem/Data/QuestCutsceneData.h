#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "QuestCutsceneData.generated.h"

UENUM(BlueprintType)
enum class EQuestCutsceneTrigger : uint8
{
	OnQuestStart UMETA(DisplayName = "On Quest Start"),
	OnStageStart UMETA(DisplayName = "On Stage Start"),
	OnStageComplete UMETA(DisplayName = "On Stage Complete"),
	OnQuestComplete UMETA(DisplayName = "On Quest Complete"),
	OnQuestFail UMETA(DisplayName = "On Quest Fail")
};

/**
 * Quest Cutscene Data
 * 
 * Defines a cutscene to play during quest progression
 */
USTRUCT(BlueprintType)
struct FQuestCutsceneData
{
	GENERATED_BODY()

	/** Level sequence to play */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cutscene")
	TObjectPtr<ULevelSequence> Sequence;

	/** When to trigger this cutscene */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cutscene")
	EQuestCutsceneTrigger TriggerTiming = EQuestCutsceneTrigger::OnQuestStart;

	/** Can player skip this cutscene? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cutscene")
	bool bCanSkip = true;

	/** Background music during cutscene */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cutscene|Audio")
	TObjectPtr<USoundBase> BackgroundMusic;
};
