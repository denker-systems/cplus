// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDefinition.h"
#include "QuestProgressWidget.generated.h"

class UQuestTask;
class UTextBlock;
class UProgressBar;
class UVerticalBox;

/**
 * Quest Progress Widget
 * 
 * Displays detailed progress for a single quest including:
 * - Quest title and description
 * - All objectives with their tasks
 * - Progress bars and counters for each task
 * - Task type indicators (Collect, Kill, Interact, Explore, Talk)
 * 
 * Blueprint-extendable for custom styling
 */
UCLASS()
class CPLUS_API UQuestProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Set the quest to display
	 * @param Quest The quest definition to show
	 * @param CurrentObjectiveIndex Which objective is currently active
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void SetQuest(UQuestDefinition* Quest, int32 CurrentObjectiveIndex = 0);

	/**
	 * Refresh the progress display
	 * Call this when task progress updates
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void RefreshProgress();

	/**
	 * Get formatted text for a task's progress
	 * @param Task The task to format
	 * @return Formatted text (e.g., "Collect: 3/10 Gold Coins")
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest UI")
	static FText GetTaskProgressText(UQuestTask* Task);

	/**
	 * Get color for task based on completion status
	 * @param Task The task to check
	 * @return Color (green if complete, yellow if in progress, white if not started)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest UI")
	static FLinearColor GetTaskColor(UQuestTask* Task);

	/**
	 * Get icon name for task type
	 * @param Task The task to check
	 * @return Icon name for UI (e.g., "Collect", "Kill", "Interact")
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest UI")
	static FText GetTaskTypeIcon(UQuestTask* Task);

protected:
	/** The quest currently being displayed */
	UPROPERTY(BlueprintReadOnly, Category = "Quest UI")
	TObjectPtr<UQuestDefinition> CurrentQuest;

	/** Current objective index */
	UPROPERTY(BlueprintReadOnly, Category = "Quest UI")
	int32 CurrentObjectiveIdx = 0;

	// Blueprint-bindable widgets (set in Blueprint)
	
	/** Quest title text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Quest UI")
	TObjectPtr<UTextBlock> QuestTitleText;

	/** Quest description text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Quest UI")
	TObjectPtr<UTextBlock> QuestDescriptionText;

	/** Container for objective entries */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Quest UI")
	TObjectPtr<UVerticalBox> ObjectivesContainer;

	/**
	 * Blueprint event called when quest is set
	 * Override in Blueprint to customize display
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest UI")
	void OnQuestSet(UQuestDefinition* Quest);

	/**
	 * Blueprint event called when progress is refreshed
	 * Override in Blueprint to customize updates
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest UI")
	void OnProgressRefreshed();
};
