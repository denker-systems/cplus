#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestGiverWidget.generated.h"

class UQuestDefinition;
class UTextBlock;
class UButton;
class UVerticalBox;
class UImage;

/**
 * Quest Giver Widget
 * 
 * UI widget shown when NPC offers a quest to the player.
 * Displays quest information and Accept/Decline buttons.
 * 
 * Fully implemented in C++ - no Blueprint required
 */
UCLASS()
class CPLUS_API UQuestGiverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Initialize widget with quest data
	 * 
	 * @param Quest The quest being offered
	 * @param QuestGiver The NPC offering the quest
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void InitializeWidget(UQuestDefinition* Quest, AActor* QuestGiver);

protected:
	/** Called when widget is constructed */
	virtual void NativeConstruct() override;

	/** The quest being offered */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TObjectPtr<UQuestDefinition> CurrentQuest;

	/** The NPC offering the quest */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TObjectPtr<AActor> CurrentQuestGiver;

	// UI Components (can be set in Blueprint or created in C++)
	
	/** Quest title text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	/** Quest description text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

	/** Accept button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AcceptButton;

	/** Decline button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DeclineButton;

	/** Quest icon image (optional) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> QuestIcon;

private:
	/** Called when Accept button is clicked */
	UFUNCTION()
	void OnAcceptClicked();

	/** Called when Decline button is clicked */
	UFUNCTION()
	void OnDeclineClicked();

	/** Update UI with quest data */
	void UpdateQuestDisplay();
};
