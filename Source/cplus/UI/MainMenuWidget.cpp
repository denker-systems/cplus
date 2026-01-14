#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SaveSystem/SaveGameSubsystem.h"
#include "Core/BaseGameMode.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: NativeConstruct called"));

	// Set title
	if (TitleText)
	{
		TitleText->SetText(FText::FromString("CPLUS"));
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Title set"));
	}

	// Bind buttons
	if (NewGameButton)
	{
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: New Game button bound"));
	}

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinueClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Continue button bound"));
	}

	if (LoadGameButton)
	{
		LoadGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGameClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Load Game button bound"));
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Settings button bound"));
	}

	if (ExitGameButton)
	{
		ExitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitGameClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Exit Game button bound"));
	}

	// Update button states
	UpdateButtonStates();

	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: NativeConstruct complete"));
}

void UMainMenuWidget::UpdateButtonStates()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Updating button states"));

	// Check if any saves exist
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>();
	if (!SaveSub)
	{
		return;
	}

	// Check if any save exists
	bool bAnySaveExists = false;
	for (int32 i = 0; i < USaveGameSubsystem::MaxSaveSlots; ++i)
	{
		if (SaveSub->DoesSaveExist(i))
		{
			bAnySaveExists = true;
			break;
		}
	}

	// Enable/disable Continue button based on save existence
	if (ContinueButton)
	{
		ContinueButton->SetIsEnabled(bAnySaveExists);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Continue button %s"), 
			bAnySaveExists ? TEXT("enabled") : TEXT("disabled"));
	}

	// Load Game button always enabled (will show "no saves" message if none exist)
	if (LoadGameButton)
	{
		LoadGameButton->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnNewGameClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: New Game clicked"));

	// TODO: Show slot selection dialog
	// For now, just start new game in slot 0
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>();
	if (!SaveSub)
	{
		return;
	}

	// Start new game in slot 0
	SaveSub->NewGame(0, TEXT("Player"));

	// Hide Main Menu and start gameplay
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		if (ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(GameMode))
		{
			BaseGameMode->HideMainMenu();
		}
	}
}

void UMainMenuWidget::OnContinueClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Continue clicked"));

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>();
	if (!SaveSub)
	{
		return;
	}

	// Load most recent save
	if (SaveSub->Continue())
	{
		// Load game level
		UGameplayStatics::OpenLevel(this, FName("Lvl_Denker"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> MAIN MENU: No saves found to continue"));
	}
}

void UMainMenuWidget::OnLoadGameClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Load Game clicked"));

	// TODO: Show save slot selection dialog
	// For now, just load from slot 0
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>();
	if (!SaveSub)
	{
		return;
	}

	// Try to load from slot 0
	if (SaveSub->LoadGame(0))
	{
		// Load game level
		UGameplayStatics::OpenLevel(this, FName("Lvl_Denker"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> MAIN MENU: No save in slot 0"));
	}
}

void UMainMenuWidget::OnSettingsClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Settings clicked"));

	// TODO: Show settings menu
	UE_LOG(LogTemp, Warning, TEXT(">>> MAIN MENU: Settings not implemented yet"));
}

void UMainMenuWidget::OnExitGameClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: Exit Game clicked"));

	// Quit application
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UMainMenuWidget::ShowNewGameSlotSelection()
{
	// TODO: Implement slot selection dialog
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: ShowNewGameSlotSelection - TODO"));
}

void UMainMenuWidget::ShowLoadGameSlotSelection()
{
	// TODO: Implement slot selection dialog
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU: ShowLoadGameSlotSelection - TODO"));
}
