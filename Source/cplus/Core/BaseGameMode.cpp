// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseGameMode.h"
#include "BasePlayerCharacter.h"
#include "BasePlayerController.h"
#include "UI/MainMenuActor.h"
#include "UI/UIManager.h"
#include "QuestSystem/UI/QuestUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABaseGameMode::ABaseGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseGameMode: Constructor called"));
	
	// Set default pawn class to BasePlayerCharacter
	DefaultPawnClass = ABasePlayerCharacter::StaticClass();
	
	// Set default player controller class to BasePlayerController
	PlayerControllerClass = ABasePlayerController::StaticClass();
	
	UE_LOG(LogTemp, Warning, TEXT("BaseGameMode: DefaultPawnClass = %s"), DefaultPawnClass ? *DefaultPawnClass->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BaseGameMode: PlayerControllerClass = %s"), PlayerControllerClass ? *PlayerControllerClass->GetName() : TEXT("NULL"));
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: BeginPlay called"));
	
	// Find or spawn Main Menu Actor
	FindOrSpawnMainMenuActor();
	
	// Show Main Menu if enabled
	if (bShowMainMenuOnStart)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Showing Main Menu on start"));
		ShowMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Main Menu disabled (development mode)"));
	}
}

void ABaseGameMode::FindOrSpawnMainMenuActor()
{
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Finding or spawning Main Menu Actor"));

	// First, try to find existing MainMenuActor in level
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainMenuActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		MainMenuActor = Cast<AMainMenuActor>(FoundActors[0]);
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Found existing MainMenuActor in level: %s"), 
			*MainMenuActor->GetName());
		return;
	}

	// If not found and we have a class set, spawn one
	if (MainMenuActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = FName(TEXT("MainMenuActor"));
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		MainMenuActor = GetWorld()->SpawnActor<AMainMenuActor>(
			MainMenuActorClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (MainMenuActor)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Spawned MainMenuActor: %s"), 
				*MainMenuActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(">>> BASE GAME MODE: Failed to spawn MainMenuActor"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> BASE GAME MODE: No MainMenuActor found in level and no MainMenuActorClass set"));
		UE_LOG(LogTemp, Warning, TEXT(">>> BASE GAME MODE: Main Menu camera will not be used"));
	}
}

void ABaseGameMode::ShowMainMenu()
{
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: ShowMainMenu called"));
	
	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> BASE GAME MODE: MainMenuWidgetClass not set!"));
		UE_LOG(LogTemp, Error, TEXT(">>> BASE GAME MODE: Open BP_BaseGameMode -> Class Defaults -> Game Mode|Main Menu -> Set Main Menu Widget Class"));
		return;
	}
	
	// Activate Main Menu camera if available
	if (MainMenuActor)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Activating Main Menu camera"));
		MainMenuActor->ActivateMenuCamera();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> BASE GAME MODE: No MainMenuActor set - camera will not change"));
	}
	
	// Create Main Menu widget if not already created
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Creating Main Menu widget"));
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		
		if (!MainMenuWidget)
		{
			UE_LOG(LogTemp, Error, TEXT(">>> BASE GAME MODE: Failed to create Main Menu widget"));
			return;
		}
		
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Main Menu widget created: %s"), *MainMenuWidget->GetClass()->GetName());
	}
	
	// Add to viewport
	MainMenuWidget->AddToViewport(1000); // High Z-order to be on top
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Main Menu added to viewport"));
	
	// Hide Quest UI when in Main Menu
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(PC->GetPawn()))
		{
			if (UUIManager* UIManager = Player->GetUIManager())
			{
				if (UQuestUIManager* QuestUI = UIManager->GetQuestUIManager())
				{
					QuestUI->HideUI();
					UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Quest UI hidden"));
				}
			}
		}
	}
	
	// Set input mode to UI
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Input mode set to UI"));
	}
	
	// Pause game while in Main Menu
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Game paused"));
	
	// Set Main Menu active flag
	bIsMainMenuActive = true;
}

void ABaseGameMode::HideMainMenu()
{
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: HideMainMenu called"));
	
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> BASE GAME MODE: No Main Menu widget to hide"));
		return;
	}
	
	// Deactivate Main Menu camera if available
	if (MainMenuActor)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Deactivating Main Menu camera"));
		MainMenuActor->DeactivateMenuCamera();
	}
	
	// Remove from viewport
	MainMenuWidget->RemoveFromParent();
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Main Menu removed from viewport"));
	
	// Restore Quest UI visibility
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(PC->GetPawn()))
		{
			if (UUIManager* UIManager = Player->GetUIManager())
			{
				if (UQuestUIManager* QuestUI = UIManager->GetQuestUIManager())
				{
					QuestUI->ShowUI();
					UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Quest UI restored"));
				}
			}
		}
	}
	
	// Set input mode back to game
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Input mode set to Game"));
	}
	
	// Unpause game
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UE_LOG(LogTemp, Display, TEXT(">>> BASE GAME MODE: Game unpaused"));
	
	// Clear Main Menu active flag
	bIsMainMenuActive = false;
}
