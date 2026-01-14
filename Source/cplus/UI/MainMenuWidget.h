#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

/**
 * Main Menu Widget
 * 
 * Main menu screen with:
 * - New Game (starts new game in slot selection)
 * - Continue (loads most recent save)
 * - Load Game (shows save slot selection)
 * - Settings (opens settings menu)
 * - Exit Game (quits application)
 * 
 * Fully implemented in C++ - minimal Blueprint required
 */
UCLASS()
class CPLUS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	// ===== UI COMPONENTS =====

	/** Menu title text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	/** Button container */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> ButtonContainer;

	/** New Game button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NewGameButton;

	/** Continue button (disabled if no saves) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	/** Load Game button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> LoadGameButton;

	/** Settings button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> SettingsButton;

	/** Exit Game button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ExitGameButton;

	// ===== BUTTON HANDLERS =====

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnLoadGameClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnExitGameClicked();

	// ===== HELPER FUNCTIONS =====

	/** Update button states based on save existence */
	void UpdateButtonStates();

	/** Show save slot selection for new game */
	void ShowNewGameSlotSelection();

	/** Show save slot selection for load game */
	void ShowLoadGameSlotSelection();
};
