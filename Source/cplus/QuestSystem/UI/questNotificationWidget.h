// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDefinition.h"
#include "questNotificationWidget.generated.h"

/**
 * Quest Notification Widget
 * 
 * C++ base class for quest notification popups AND quest tracker.
 * 
 * Two modes:
 * 1. Notification Mode - Temporary popups (auto-hide after duration)
 * 2. Tracker Mode - Persistent quest progress display (stays visible)
 * 
 * Shows quest start, progress, and completion notifications.
 */
UCLASS()
class CPLUS_API UQuestNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ===== NOTIFICATION MODE (Temporary Popups) =====
	
	/**
	 * Show quest started notification
	 * @param Quest The quest that was started
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Notification")
	void ShowQuestStarted(UQuestDefinition* Quest);

	/**
	 * Show quest completed notification
	 * @param Quest The quest that was completed
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Notification")
	void ShowQuestCompleted(UQuestDefinition* Quest);

	/**
	 * Show quest failed notification
	 * @param Quest The quest that failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Notification")
	void ShowQuestFailed(UQuestDefinition* Quest);

	/**
	 * Show objective updated notification
	 * @param Quest The quest with updated objective
	 * @param ObjectiveIndex Index of the updated objective
	 * @param CurrentProgress Current progress value
	 * @param TargetProgress Target progress value
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Notification")
	void ShowObjectiveUpdated(UQuestDefinition* Quest, int32 ObjectiveIndex, int32 CurrentProgress, int32 TargetProgress);

	// ===== TRACKER MODE (Persistent Display) =====
	
	/**
	 * Start tracking a quest (persistent display)
	 * @param Quest The quest to track
	 * @param CurrentObjectiveIndex Current objective index
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Tracker")
	void TrackQuest(UQuestDefinition* Quest, int32 CurrentObjectiveIndex = 0);

	/**
	 * Stop tracking the current quest
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Tracker")
	void UntrackQuest();

	/**
	 * Update tracked quest progress
	 * Call when task progress changes
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Tracker")
	void UpdateTrackedProgress();

	/**
	 * Is a quest currently being tracked?
	 */
	UFUNCTION(BlueprintPure, Category = "Quest Tracker")
	bool IsTrackingQuest() const { return bIsTracking; }

	/**
	 * Get the currently tracked quest
	 */
	UFUNCTION(BlueprintPure, Category = "Quest Tracker")
	UQuestDefinition* GetTrackedQuest() const { return TrackedQuest; }

	// ===== SETTINGS =====
	
	/** Should new quests auto-track? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Tracker")
	bool bAutoTrackNewQuests = true;

	/** Duration to show notification (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Notification")
	float NotificationDuration = 3.0f;

	/** Should notification auto-hide after duration? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Notification")
	bool bAutoHide = true;

	// ===== WIDGET BINDINGS (Bind these in Blueprint Designer) =====
	
	/** Main tracker container - set visibility to show/hide tracker */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UPanelWidget* TrackerContainer;

	/** Quest title text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* QuestTitleText;

	/** Objective description text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ObjectiveText;

	/** Container for task entries */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UPanelWidget* TasksContainer;

protected:
	// ===== INTERNAL HELPER FUNCTIONS =====
	
	/**
	 * Populate tracker with quest data
	 * Called automatically by TrackQuest and UpdateTrackedProgress
	 */
	void PopulateTrackerContent();

	/**
	 * Get formatted quest title text
	 */
	FText GetQuestTitleText() const;

	/**
	 * Get formatted objective text
	 */
	FText GetObjectiveText() const;

	/**
	 * Get color for task based on completion status
	 */
	FLinearColor GetTaskColor(UQuestTask* Task) const;
	/**
	 * Called when quest started notification should be displayed
	 * Override in Blueprint to customize notification appearance
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Notification")
	void OnQuestStarted(const FText& QuestTitle);

	/**
	 * Called when quest completed notification should be displayed
	 * Override in Blueprint to customize notification appearance
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Notification")
	void OnQuestCompleted(const FText& QuestTitle);

	/**
	 * Called when quest failed notification should be displayed
	 * Override in Blueprint to customize notification appearance
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Notification")
	void OnQuestFailed(const FText& QuestTitle);

	/**
	 * Called when objective updated notification should be displayed
	 * Override in Blueprint to customize notification appearance
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Notification")
	void OnObjectiveUpdated(const FText& ObjectiveDescription, int32 Current, int32 Target);

	// ===== TRACKER BLUEPRINT EVENTS =====
	
	/**
	 * Called when quest tracking starts
	 * Override in Blueprint to show tracker UI
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Tracker")
	void OnQuestTracked(UQuestDefinition* Quest, int32 ObjectiveIndex);

	/**
	 * Called when quest tracking stops
	 * Override in Blueprint to hide tracker UI
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Tracker")
	void OnQuestUntracked();

	/**
	 * Called when tracked quest progress updates
	 * Override in Blueprint to update tracker display
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Tracker")
	void OnTrackedProgressUpdated(UQuestDefinition* Quest, int32 ObjectiveIndex);

private:
	/** Currently tracked quest */
	UPROPERTY()
	TObjectPtr<UQuestDefinition> TrackedQuest;

	/** Current objective index of tracked quest */
	UPROPERTY()
	int32 TrackedObjectiveIndex = 0;

	/** Is currently tracking a quest? */
	bool bIsTracking = false;
};
