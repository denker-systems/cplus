// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestSystem/Runtime/QuestSubSystem.h"
#include "SaveSystem/SaveGameSubsystem.h"
#include "Core/BaseGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Components/VerticalBoxSlot.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: NativeConstruct called"));
	
	// Set title text
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(TEXT("PAUSED")));
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Title text set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> PAUSE MENU: TitleText is NULL!"));
	}
	
	// Create menu buttons automatically
	CreateMenuButtons();
	
	// Hide by default
	SetVisibility(ESlateVisibility::Hidden);
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: NativeConstruct complete"));
}

void UPauseMenuWidget::CreateMenuButtons()
{
	if (!MenuContainer)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> PAUSE MENU: MenuContainer is NULL! Cannot create buttons."));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Creating menu buttons"));
	
	// Clear any existing buttons
	MenuContainer->ClearChildren();
	CreatedButtons.Empty();
	
	// Create Resume button
	UButton* ResumeButton = CreateMenuButton(FText::FromString(TEXT("Resume Game")), FName("OnResumeClicked"));
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
		CreatedButtons.Add(ResumeButton);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Resume button created"));
	}
	
	// Create Save Game button
	UButton* SaveButton = CreateMenuButton(FText::FromString(TEXT("Save Game")), FName("OnSaveGameClicked"));
	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveGameClicked);
		CreatedButtons.Add(SaveButton);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Save Game button created"));
	}
	
	// Create Load Game button
	UButton* LoadButton = CreateMenuButton(FText::FromString(TEXT("Load Game")), FName("OnLoadGameClicked"));
	if (LoadButton)
	{
		LoadButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnLoadGameClicked);
		CreatedButtons.Add(LoadButton);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Load Game button created"));
	}
	
	// Create Main Menu button
	UButton* MainMenuButton = CreateMenuButton(FText::FromString(TEXT("Main Menu")), FName("OnMainMenuClicked"));
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
		CreatedButtons.Add(MainMenuButton);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Main Menu button created"));
	}
	
	// Create Reset button
	UButton* ResetButton = CreateMenuButton(FText::FromString(TEXT("Reset Game")), FName("OnResetClicked"));
	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResetClicked);
		CreatedButtons.Add(ResetButton);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Reset button created"));
	}
	
	// Create Quit button
	UButton* QuitButton = CreateMenuButton(FText::FromString(TEXT("Quit Game")), FName("OnQuitClicked"));
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
		CreatedButtons.Add(QuitButton);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Quit button created"));
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Created %d button(s)"), CreatedButtons.Num());
}

UButton* UPauseMenuWidget::CreateMenuButton(const FText& ButtonText, const FName& FunctionName)
{
	// Create button
	UButton* Button = NewObject<UButton>(this);
	if (!Button)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> PAUSE MENU: Failed to create button"));
		return nullptr;
	}
	
	// Create text block for button label
	UTextBlock* ButtonLabel = NewObject<UTextBlock>(this);
	if (ButtonLabel)
	{
		ButtonLabel->SetText(ButtonText);
		ButtonLabel->SetJustification(ETextJustify::Center);
		
		// Set text style
		FSlateFontInfo FontInfo = ButtonLabel->GetFont();
		FontInfo.Size = 24;
		ButtonLabel->SetFont(FontInfo);
		ButtonLabel->SetColorAndOpacity(FLinearColor::White);
		
		// Add text to button
		Button->AddChild(ButtonLabel);
	}
	
	// Add button to container
	UVerticalBoxSlot* ButtonSlot = MenuContainer->AddChildToVerticalBox(Button);
	if (ButtonSlot)
	{
		// Set padding between buttons
		ButtonSlot->SetPadding(FMargin(0.0f, 10.0f, 0.0f, 10.0f));
		ButtonSlot->SetHorizontalAlignment(HAlign_Center);
		ButtonSlot->SetVerticalAlignment(VAlign_Center);
	}
	
	return Button;
}

void UPauseMenuWidget::ShowPauseMenu()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Showing pause menu"));
	
	// Set visibility
	SetVisibility(ESlateVisibility::Visible);
	
	// Pause game
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	// Set input mode to UI
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	
	// Call Blueprint event
	OnPauseMenuShown();
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Game paused, input mode set to UI"));
}

void UPauseMenuWidget::HidePauseMenu()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Hiding pause menu"));
	
	// Set visibility
	SetVisibility(ESlateVisibility::Hidden);
	
	// Unpause game
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	// Set input mode back to game
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
	
	// Call Blueprint event
	OnPauseMenuHidden();
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Game resumed, input mode set to Game"));
}

void UPauseMenuWidget::TogglePauseMenu()
{
	if (IsPauseMenuVisible())
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

bool UPauseMenuWidget::IsPauseMenuVisible() const
{
	return GetVisibility() == ESlateVisibility::Visible;
}

void UPauseMenuWidget::OnResumeClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Resume button clicked"));
	HidePauseMenu();
}

void UPauseMenuWidget::OnSaveGameClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Save Game button clicked"));
	
	// Get SaveGameSubsystem
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>())
		{
			// Quick save to current slot (or slot 0 if no active slot)
			int32 SlotToSave = SaveSub->GetCurrentSaveSlot();
			if (SlotToSave < 0)
			{
				SlotToSave = 0; // Default to slot 0 if no active slot
			}
			
			if (SaveSub->SaveGame(SlotToSave))
			{
				UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Game saved to slot %d"), SlotToSave);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT(">>> PAUSE MENU: Failed to save game"));
			}
		}
	}
	
	// Resume game after save
	HidePauseMenu();
}

void UPauseMenuWidget::OnLoadGameClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Load Game button clicked"));
	
	// Get SaveGameSubsystem
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>())
		{
			// Load from current slot (or slot 0 if no active slot)
			int32 SlotToLoad = SaveSub->GetCurrentSaveSlot();
			if (SlotToLoad < 0)
			{
				SlotToLoad = 0; // Default to slot 0 if no active slot
			}
			
			if (SaveSub->LoadGame(SlotToLoad))
			{
				UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Game loaded from slot %d"), SlotToLoad);
				
				// Reload level to apply loaded state
				UGameplayStatics::OpenLevel(this, FName("Lvl_Denker"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT(">>> PAUSE MENU: No save found in slot %d"), SlotToLoad);
			}
		}
	}
}

void UPauseMenuWidget::OnMainMenuClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Main Menu button clicked"));
	
	// Hide pause menu first
	HidePauseMenu();
	
	// Show Main Menu via GameMode
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		if (ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(GameMode))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Showing Main Menu"));
			BaseGameMode->ShowMainMenu();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(">>> PAUSE MENU: GameMode is not BaseGameMode"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> PAUSE MENU: No GameMode found"));
	}
}

void UPauseMenuWidget::OnResetClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Reset button clicked"));
	
	// Get QuestSubsystem and reset all quests
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UQuestSubSystem* QuestSub = GameInstance->GetSubsystem<UQuestSubSystem>())
		{
			QuestSub->ResetAllQuests();
			UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: All quests reset"));
		}
	}
	
	// IMPORTANT: Reset input mode to Game before reload
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
		UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Input mode reset to Game"));
	}
	
	// IMPORTANT: Unpause game before reloading level
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Game unpaused before reload"));
	
	// Reload current level
	UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())));
	
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Level reloading..."));
}

void UPauseMenuWidget::OnQuitClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> PAUSE MENU: Quit button clicked"));
	
	// Quit game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
