// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDefinition.h"
#include "questNotificationWidget.generated.h"

/**
 * Quest Notification Widget
 * 
 * C++ base class for quest notification popups.
 * Shows quest start, progress, and completion notifications.
 */
UCLASS()
class CPLUS_API UQuestNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
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

protected:
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
	void OnObjectiveUpdated(const FText& ObjectiveText, int32 Current, int32 Target);

	/** Duration to show notification (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Notification")
	float NotificationDuration = 3.0f;

	/** Should notification auto-hide after duration? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Notification")
	bool bAutoHide = true;
};
