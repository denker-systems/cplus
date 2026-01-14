#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSystem/Data/QuestReward.h"
#include "QuestCompletionWidget.generated.h"

class UQuestDefinition;
class UTextBlock;
class UButton;
class UVerticalBox;
class UImage;

/**
 * Quest Completion Widget
 * 
 * UI widget shown when player turns in a completed quest to NPC.
 * Displays quest completion message, rewards, and Continue button.
 * 
 * Fully implemented in C++ - minimal Blueprint required
 */
UCLASS()
class CPLUS_API UQuestCompletionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Initialize widget with completed quest data
	 * 
	 * @param Quest The quest that was completed
	 * @param Rewards The rewards earned
	 * @param QuestGiver The NPC who gave the quest
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void InitializeWidget(UQuestDefinition* Quest, const FQuestReward& Rewards, AActor* QuestGiver);

	/**
	 * Delegate called when player clicks Continue button
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinueClicked);
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnContinueClicked OnContinueClicked;

protected:
	/** Called when widget is constructed */
	virtual void NativeConstruct() override;

	/** The completed quest */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TObjectPtr<UQuestDefinition> CompletedQuest;

	/** The rewards earned */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FQuestReward QuestRewards;

	/** The NPC who gave the quest */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TObjectPtr<AActor> QuestGiver;

	// UI Components (bind in Blueprint)
	
	/** "Quest Completed!" title text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CompletionTitleText;

	/** Quest name text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitleText;

	/** Quest description/completion message */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CompletionMessageText;

	/** Rewards container */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> RewardsContainer;

	/** XP reward text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> XPRewardText;

	/** Gold reward text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldRewardText;

	/** Reputation reward text (optional) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ReputationRewardText;

	/** Continue button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	/** Quest completion icon (optional) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> CompletionIcon;

private:
	/** Called when Continue button is clicked */
	UFUNCTION()
	void OnContinueButtonClicked();

	/** Update UI with quest completion data */
	void UpdateCompletionDisplay();

	/** Close the widget and restore game input */
	void CloseWidget();
};
