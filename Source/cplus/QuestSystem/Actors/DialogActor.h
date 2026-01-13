#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestDialogData.h"
#include "DialogActor.generated.h"

/**
 * Dialog Actor
 * 
 * Handles dialog playback for quests (audio, animations, UI)
 */
UCLASS()
class CPLUS_API ADialogActor : public AActor
{
	GENERATED_BODY()

public:
	ADialogActor();

	/**
	 * Play dialog sequence
	 * @param Dialogs Array of dialogs to play
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void PlayDialogSequence(const TArray<FQuestDialogData>& Dialogs);

	/**
	 * Stop current dialog
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void StopDialog();

protected:
	/** Audio component for voice over */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog")
	class UAudioComponent* AudioComponent;

	/** Current dialog index */
	UPROPERTY(BlueprintReadOnly, Category = "Dialog")
	int32 CurrentDialogIndex = 0;

	/** Current dialog sequence */
	UPROPERTY(BlueprintReadOnly, Category = "Dialog")
	TArray<FQuestDialogData> CurrentDialogs;

	/** Play next dialog in sequence */
	void PlayNextDialog();

	/** Timer handle for dialog duration */
	FTimerHandle DialogTimerHandle;
};
