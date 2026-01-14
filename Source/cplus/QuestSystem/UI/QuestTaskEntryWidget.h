// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTaskEntryWidget.generated.h"

class UQuestTask;
class UTextBlock;
class UProgressBar;
class UImage;

/**
 * Quest Task Entry Widget
 * 
 * Displays a single task within a quest objective.
 * Shows:
 * - Task type icon (Collect, Kill, Interact, Explore, Talk)
 * - Task description
 * - Progress counter (3/10)
 * - Progress bar (visual representation)
 * - Completion status (color-coded)
 * 
 * Blueprint-extendable for custom styling
 */
UCLASS()
class CPLUS_API UQuestTaskEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Set the task to display
	 * @param Task The task to show
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void SetTask(UQuestTask* Task);

	/**
	 * Refresh the task display
	 * Call when task progress updates
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void RefreshTask();

protected:
	/** The task currently being displayed */
	UPROPERTY(BlueprintReadOnly, Category = "Quest UI")
	TObjectPtr<UQuestTask> CurrentTask;

	// Blueprint-bindable widgets (optional - set in Blueprint)
	
	/** Task type icon/text (e.g., "Collect", "Kill") */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Quest UI")
	TObjectPtr<UTextBlock> TaskTypeText;

	/** Task description text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Quest UI")
	TObjectPtr<UTextBlock> TaskDescriptionText;

	/** Progress counter text (e.g., "3/10") */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Quest UI")
	TObjectPtr<UTextBlock> ProgressCounterText;

	/** Progress bar visual */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Quest UI")
	TObjectPtr<UProgressBar> ProgressBar;

	/** Task type icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Quest UI")
	TObjectPtr<UImage> TaskTypeIcon;

	/**
	 * Blueprint event called when task is set
	 * Override in Blueprint to customize display
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest UI")
	void OnTaskSet(UQuestTask* Task);

	/**
	 * Blueprint event called when task is refreshed
	 * Override in Blueprint to customize updates
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest UI")
	void OnTaskRefreshed();
};
