// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestDefinition.h"
#include "QuestReward.h"
#include "QuestUIManager.generated.h"

class UQuestNotificationWidget;
class UQuestJournalWidget;
class UQuestSubSystem;

/**
 * Quest UI Manager
 * 
 * Manages quest-related UI widgets and binds them to QuestSubsystem.
 * Attach to PlayerController or HUD to handle quest UI.
 * 
 * Features:
 * - Automatic quest notifications (start/complete/fail)
 * - Quest journal management (open/close)
 * - Binds to QuestSubsystem delegates automatically
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestUIManager();

	// ===== WIDGET CLASSES =====
	
	/** Widget class for quest notifications */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI")
	TSubclassOf<UQuestNotificationWidget> NotificationWidgetClass;

	/** Widget class for quest journal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI")
	TSubclassOf<UQuestJournalWidget> JournalWidgetClass;

	// ===== SETTINGS =====
	
	/** Auto-show notification when quest starts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI|Settings")
	bool bAutoShowQuestStarted = true;

	/** Auto-show notification when quest completes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI|Settings")
	bool bAutoShowQuestCompleted = true;

	/** Auto-show notification when objective updates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI|Settings")
	bool bAutoShowObjectiveUpdated = false;

	// ===== API - NOTIFICATIONS =====
	
	/**
	 * Show quest started notification
	 * @param QuestID The quest that started
	 * @param Quest The quest definition
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void ShowQuestStartedNotification(FName QuestID, UQuestDefinition* Quest);

	/**
	 * Show quest completed notification
	 * @param QuestID The quest that completed
	 * @param Rewards The rewards received
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void ShowQuestCompletedNotification(FName QuestID, const FQuestReward& Rewards);

	/**
	 * Show objective updated notification
	 * @param QuestID The quest with updated objective
	 * @param ObjectiveIndex Index of updated objective
	 * @param TaskIndex Index of updated task
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void ShowObjectiveUpdatedNotification(FName QuestID, int32 ObjectiveIndex, int32 TaskIndex);

	// ===== API - JOURNAL =====
	
	/**
	 * Open quest journal
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void OpenQuestJournal();

	/**
	 * Close quest journal
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void CloseQuestJournal();

	/**
	 * Toggle quest journal (open/close)
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void ToggleQuestJournal();

	/**
	 * Is quest journal currently open?
	 */
	UFUNCTION(BlueprintPure, Category = "Quest UI")
	bool IsJournalOpen() const { return bIsJournalOpen; }

	/**
	 * Hide all quest UI (for Main Menu, etc.)
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void HideUI();

	/**
	 * Show all quest UI (restore after Main Menu, etc.)
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void ShowUI();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Bind to QuestSubsystem delegates */
	void BindToQuestSubsystem();

	/** Unbind from QuestSubsystem delegates */
	void UnbindFromQuestSubsystem();

	/** Create notification widget instance */
	void CreateNotificationWidget();

	/** Create journal widget instance */
	void CreateJournalWidget();

	// ===== DELEGATE HANDLERS =====
	
	/** Called when quest starts */
	UFUNCTION()
	void HandleQuestStarted(FName QuestID, UQuestDefinition* Quest);

	/** Called when quest completes */
	UFUNCTION()
	void HandleQuestCompleted(FName QuestID, const FQuestReward& Rewards);

	/** Called when quest task updates */
	UFUNCTION()
	void HandleQuestTaskUpdated(FName QuestID, int32 ObjectiveIndex, int32 TaskIndex);

private:
	/** Notification widget instance */
	UPROPERTY()
	TObjectPtr<UQuestNotificationWidget> NotificationWidget;

	/** Journal widget instance */
	UPROPERTY()
	TObjectPtr<UQuestJournalWidget> JournalWidget;

	/** Cached reference to QuestSubsystem */
	UPROPERTY()
	TObjectPtr<UQuestSubSystem> QuestSubsystem;

	/** Is journal currently open? */
	bool bIsJournalOpen = false;
};
