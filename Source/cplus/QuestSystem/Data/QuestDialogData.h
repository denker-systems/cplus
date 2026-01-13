#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "Engine/Texture2D.h"
#include "QuestDialogData.generated.h"

/**
 * Quest Dialog Data
 * 
 * Represents a single dialog entry in a quest
 */
USTRUCT(BlueprintType)
struct FQuestDialogData
{
	GENERATED_BODY()

	/** Speaker identifier (NPC name or ID) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog")
	FName SpeakerID;

	/** Speaker display name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog")
	FText SpeakerName;

	/** Dialog text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog", meta = (MultiLine = true))
	FText DialogText;

	/** Voice over audio */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog|Audio")
	TObjectPtr<USoundBase> VoiceOver;

	/** Speaker portrait image */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog|Visual")
	TObjectPtr<UTexture2D> SpeakerPortrait;

	/** Animation to play during dialog */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog|Visual")
	TObjectPtr<UAnimMontage> DialogAnimation;

	/** Duration to display dialog (0 = wait for player input) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog|Timing")
	float DisplayDuration = 0.0f;
};
