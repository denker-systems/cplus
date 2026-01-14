// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIManager.generated.h"

class UPauseMenuWidget;
class UQuestUIManager;

/**
 * UI Manager
 * 
 * Central manager for ALL UI in the game
 * Manages: Pause Menu, Quest UI, Inventory, Settings, HUD, etc.
 * 
 * Attach to PlayerController or PlayerCharacter
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UUIManager();

	// ===== WIDGET CLASSES =====
	
	/** Widget class for pause menu */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Manager")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	// ===== SUB-MANAGERS =====
	
	/** Quest UI manager (handles quest notifications and journal) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI Manager")
	UQuestUIManager* QuestUIManager;

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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Create pause menu widget */
	void CreatePauseMenuWidget();

private:
	/** Pause menu widget instance */
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;
};
