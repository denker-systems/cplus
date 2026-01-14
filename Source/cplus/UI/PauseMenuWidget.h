// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "PauseMenuWidget.generated.h"

/**
 * Pause Menu Widget
 * 
 * Auto-creates pause menu UI with Resume, Reset, and Quit buttons
 * All logic handled in C++ - Blueprint only needs to bind MenuContainer
 */
UCLASS()
class CPLUS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Show the pause menu
	 */
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ShowPauseMenu();

	/**
	 * Hide the pause menu
	 */
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void HidePauseMenu();

	/**
	 * Toggle pause menu visibility
	 */
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void TogglePauseMenu();

	/**
	 * Is pause menu currently visible?
	 */
	UFUNCTION(BlueprintPure, Category = "Pause Menu")
	bool IsPauseMenuVisible() const;

protected:
	virtual void NativeConstruct() override;

	// ===== WIDGET BINDINGS (Bind in Blueprint Designer) =====
	
	/** Main menu container - all buttons will be added here */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* MenuContainer;

	/** Title text block */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TitleText;

	// ===== BUTTON HANDLERS =====
	
	/**
	 * Resume game
	 */
	UFUNCTION()
	void OnResumeClicked();

	/**
	 * Save current game
	 */
	UFUNCTION()
	void OnSaveGameClicked();

	/**
	 * Load saved game
	 */
	UFUNCTION()
	void OnLoadGameClicked();

	/**
	 * Return to Main Menu
	 */
	UFUNCTION()
	void OnMainMenuClicked();

	/**
	 * Quit to main menu or exit game
	 */
	UFUNCTION()
	void OnQuitClicked();

	// ===== INTERNAL HELPERS =====
	
	/**
	 * Create all menu buttons automatically
	 */
	void CreateMenuButtons();

	/**
	 * Create a single menu button
	 */
	UButton* CreateMenuButton(const FText& ButtonText, const FName& FunctionName);

	// ===== BLUEPRINT EVENTS (Optional) =====
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void OnPauseMenuShown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void OnPauseMenuHidden();

private:
	/** Created buttons for cleanup */
	UPROPERTY()
	TArray<TObjectPtr<UButton>> CreatedButtons;
};
