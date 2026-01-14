// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * Base Game Mode
 * 
 * Simple game mode that uses BasePlayerCharacter and BasePlayerController
 * Provides foundation for all game modes in the project
 * 
 * Features:
 * - Optional Main Menu overlay at game start
 * - Development mode to skip Main Menu
 */
UCLASS()
class CPLUS_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseGameMode();

	virtual void BeginPlay() override;

	// ===== MAIN MENU =====

	/** Show Main Menu at game start? (Disable for development) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode|Main Menu")
	bool bShowMainMenuOnStart = false;

	/** Main Menu widget class (Blueprint: WBP_MainMenu) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode|Main Menu")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	/** Main Menu Actor class to spawn or find in level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode|Main Menu")
	TSubclassOf<class AMainMenuActor> MainMenuActorClass;

	/** Show Main Menu overlay */
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void ShowMainMenu();

	/** Hide Main Menu overlay */
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void HideMainMenu();

protected:
	/** Current Main Menu widget instance */
	UPROPERTY()
	TObjectPtr<class UUserWidget> MainMenuWidget;

	/** Cached Main Menu Actor instance (found in level or spawned) */
	UPROPERTY()
	TObjectPtr<class AMainMenuActor> MainMenuActor;

	/** Is Main Menu currently active? */
	bool bIsMainMenuActive = false;

	/** Find or spawn Main Menu Actor */
	void FindOrSpawnMainMenuActor();

public:
	/** Check if Main Menu is currently active */
	UFUNCTION(BlueprintPure, Category = "Game Mode")
	bool IsMainMenuActive() const { return bIsMainMenuActive; }
};
