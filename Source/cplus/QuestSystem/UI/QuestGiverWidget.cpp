#include "QuestGiverWidget.h"
#include "QuestDefinition.h"
#include "QuestTrackerComponent.h"
#include "BasePlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UQuestGiverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button events
	if (AcceptButton)
	{
		AcceptButton->OnClicked.AddDynamic(this, &UQuestGiverWidget::OnAcceptClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Accept button bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST DIALOG: AcceptButton not found - make sure widget has Button named 'AcceptButton'"));
	}

	if (DeclineButton)
	{
		DeclineButton->OnClicked.AddDynamic(this, &UQuestGiverWidget::OnDeclineClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Decline button bound"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST DIALOG: DeclineButton not found - make sure widget has Button named 'DeclineButton'"));
	}
}

void UQuestGiverWidget::InitializeWidget(UQuestDefinition* Quest, AActor* QuestGiver)
{
	if (!Quest || !QuestGiver)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverWidget: InitializeWidget called with null Quest or QuestGiver"));
		return;
	}

	CurrentQuest = Quest;
	CurrentQuestGiver = QuestGiver;

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Initialized with quest [%s]"), *Quest->QuestID.ToString());

	// Update UI with quest data
	UpdateQuestDisplay();
}

void UQuestGiverWidget::UpdateQuestDisplay()
{
	if (!CurrentQuest)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverWidget: UpdateQuestDisplay called but CurrentQuest is null"));
		return;
	}

	// Update title
	if (TitleText)
	{
		TitleText->SetText(CurrentQuest->UIData.QuestTitle);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Title set to '%s'"), *CurrentQuest->UIData.QuestTitle.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST DIALOG: TitleText not found - make sure widget has TextBlock named 'TitleText'"));
	}

	// Update description
	if (DescriptionText)
	{
		DescriptionText->SetText(CurrentQuest->UIData.FullDescription);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Description set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST DIALOG: DescriptionText not found - make sure widget has TextBlock named 'DescriptionText'"));
	}

	// Update icon (optional)
	if (QuestIcon && CurrentQuest->UIData.QuestIcon)
	{
		// Set icon texture
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Quest icon set"));
	}
}

void UQuestGiverWidget::OnAcceptClicked()
{
	if (!CurrentQuest)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverWidget: OnAcceptClicked but CurrentQuest is null"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Player accepted quest [%s]"), *CurrentQuest->QuestID.ToString());

	// Get player controller
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverWidget: No owning player controller"));
		return;
	}

	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(PC->GetPawn());
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverWidget: Player is not BasePlayerCharacter"));
		return;
	}

	// Get quest tracker component
	UQuestTrackerComponent* QuestTracker = Player->GetQuestTracker();
	if (!QuestTracker)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverWidget: Player has no QuestTrackerComponent"));
		return;
	}

	// Accept the quest
	QuestTracker->AcceptQuest(CurrentQuest);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Quest accepted successfully!"));

	// Restore input mode to game
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Input mode restored to Game"));

	// Close the widget
	RemoveFromParent();
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Widget closed"));
}

void UQuestGiverWidget::OnDeclineClicked()
{
	if (CurrentQuest)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Player declined quest [%s]"), *CurrentQuest->QuestID.ToString());
	}

	// Restore input mode to game
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Input mode restored to Game"));
	}

	// Close the widget
	RemoveFromParent();
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST DIALOG: Widget closed"));
}
