// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIManager.generated.h"

class UPauseMenuWidget;
class UQuestUIManager;
class UInventoryWidget;
class UInventoryComponent;

/**
 * UI Manager
 * 
 * Central manager for ALL UI in the game
 * Manages: Pause Menu, Quest UI, Inventory, Settings, HUD, etc.
 * 
 * Attach to PlayerController or PlayerCharacter
 */
UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class CPLUS_API UUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UUIManager();

	// ===== WIDGET CLASSES =====
	
	/** Widget class for pause menu */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	// ===== QUEST UI WIDGET CLASSES (passed to QuestUIManager) =====

	/** Widget class for quest notifications */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager|Quest UI")
	TSubclassOf<class UQuestNotificationWidget> NotificationWidgetClass;

	/** Widget class for quest journal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager|Quest UI")
	TSubclassOf<class UQuestJournalWidget> JournalWidgetClass;

	/** Widget class for quest giver dialog */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager|Quest UI|Dialogs")
	TSubclassOf<class UQuestGiverWidget> QuestGiverWidgetClass;

	/** Widget class for quest completion dialog */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager|Quest UI|Dialogs")
	TSubclassOf<class UQuestCompletionWidget> QuestCompletionWidgetClass;

	/** Widget class for inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager|Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	// ===== SUB-MANAGERS =====
	
	/** Quest UI manager (handles quest notifications and journal) - created at runtime */
	UPROPERTY(BlueprintReadOnly, Category = "UI Manager")
	TObjectPtr<UQuestUIManager> QuestUIManager;

	// ===== API - PAUSE MENU =====
	
	/**
	 * Toggle pause menu (open/close)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void TogglePauseMenu();

	/**
	 * Show pause menu
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ShowPauseMenu();

	/**
	 * Hide pause menu
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void HidePauseMenu();

	/**
	 * Is pause menu currently visible?
	 */
	UFUNCTION(BlueprintPure, Category = "UI Manager")
	bool IsPauseMenuVisible() const;

	// ===== API - QUEST UI =====
	
	/**
	 * Get quest UI manager
	 */
	UFUNCTION(BlueprintPure, Category = "UI Manager")
	UQuestUIManager* GetQuestUIManager() const { return QuestUIManager; }

	// ===== API - INVENTORY =====

	/**
	 * Toggle inventory (open/close)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ToggleInventory();

	/**
	 * Show inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ShowInventory();

	/**
	 * Hide inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void HideInventory();

	/**
	 * Is inventory visible?
	 */
	UFUNCTION(BlueprintPure, Category = "UI Manager")
	bool IsInventoryVisible() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Create pause menu widget */
	void CreatePauseMenuWidget();

private:
	/** Pause menu widget instance */
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;

	/** Inventory widget instance */
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
};
