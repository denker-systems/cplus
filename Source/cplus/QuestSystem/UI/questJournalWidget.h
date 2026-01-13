// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDefinition.h"
#include "questProgressData.h"
#include "questJournalWidget.generated.h"

/**
 * Quest Journal Widget
 * 
 * C++ base class for Blueprint quest journal UI.
 * Provides data access and helper functions for displaying quests.
 */
UCLASS()
class CPLUS_API UQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Refresh journal with current quest data
	 * Called when quest state changes
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Journal")
	void RefreshJournal();

	/**
	 * Get all active quests for display
	 * @return Array of active quest progress data
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Journal")
	TArray<FQuestProgressData> GetActiveQuests() const;

	/**
	 * Get all completed quests for display
	 * @return Array of completed quest progress data
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Journal")
	TArray<FQuestProgressData> GetCompletedQuests() const;

	/**
	 * Get quest definition by ID
	 * @param QuestID The quest to retrieve
	 * @return Quest definition or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Journal")
	UQuestDefinition* GetQuestDefinition(FName QuestID) const;

protected:
	/**
	 * Called when journal is opened
	 * Override in Blueprint to setup UI
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Journal")
	void OnJournalOpened();

	/**
	 * Called when journal is closed
	 * Override in Blueprint to cleanup UI
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Journal")
	void OnJournalClosed();

	/**
	 * Called when quest data is refreshed
	 * Override in Blueprint to update UI elements
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Journal")
	void OnQuestDataRefreshed();
};
