#include "QuestCompletionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "QuestSystem/Data/QuestDefinition.h"
#include "QuestSystem/Data/QuestReward.h"
#include "Kismet/GameplayStatics.h"

void UQuestCompletionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: NativeConstruct called"));

	// Bind Continue button
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UQuestCompletionWidget::OnContinueButtonClicked);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Continue button bound"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COMPLETION: ContinueButton is NULL!"));
	}
}

void UQuestCompletionWidget::InitializeWidget(UQuestDefinition* Quest, const FQuestReward& Rewards, AActor* Giver)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: ========== InitializeWidget START =========="));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Quest = %s"), Quest ? *Quest->QuestID.ToString() : TEXT("NULL"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: XP = %d, Gold = %d"), Rewards.ExperiencePoints, Rewards.Gold);

	CompletedQuest = Quest;
	QuestRewards = Rewards;
	QuestGiver = Giver;

	if (!Quest)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COMPLETION: Quest is NULL!"));
		return;
	}

	UpdateCompletionDisplay();

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: ========== InitializeWidget END =========="));
}

void UQuestCompletionWidget::UpdateCompletionDisplay()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Updating display"));

	if (!CompletedQuest)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COMPLETION: CompletedQuest is NULL!"));
		return;
	}

	// Set completion title
	if (CompletionTitleText)
	{
		CompletionTitleText->SetText(FText::FromString("Quest Completed!"));
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Title set"));
	}

	// Set quest name
	if (QuestTitleText)
	{
		QuestTitleText->SetText(CompletedQuest->UIData.QuestTitle);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Quest title set to '%s'"), 
			*CompletedQuest->UIData.QuestTitle.ToString());
	}

	// Set completion message
	if (CompletionMessageText)
	{
		FString Message = FString::Printf(TEXT("Congratulations! You have completed '%s'."), 
			*CompletedQuest->UIData.QuestTitle.ToString());
		CompletionMessageText->SetText(FText::FromString(Message));
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Message set"));
	}

	// Set XP reward
	if (XPRewardText)
	{
		if (QuestRewards.ExperiencePoints > 0)
		{
			FText XPText = FText::Format(FText::FromString("+ {0} XP"), FText::AsNumber(QuestRewards.ExperiencePoints));
			XPRewardText->SetText(XPText);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: XP reward set: %d"), QuestRewards.ExperiencePoints);
		}
		else
		{
			XPRewardText->SetText(FText::FromString(""));
		}
	}

	// Set Gold reward
	if (GoldRewardText)
	{
		if (QuestRewards.Gold > 0)
		{
			FText GoldText = FText::Format(FText::FromString("+ {0} Gold"), FText::AsNumber(QuestRewards.Gold));
			GoldRewardText->SetText(GoldText);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Gold reward set: %d"), QuestRewards.Gold);
		}
		else
		{
			GoldRewardText->SetText(FText::FromString(""));
		}
	}

	// Set Reputation reward (optional)
	if (ReputationRewardText)
	{
		if (QuestRewards.ReputationGain > 0)
		{
			FText RepText = FText::Format(FText::FromString("+ {0} Reputation"), FText::AsNumber(QuestRewards.ReputationGain));
			ReputationRewardText->SetText(RepText);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Reputation reward set: %d"), QuestRewards.ReputationGain);
		}
		else
		{
			ReputationRewardText->SetText(FText::FromString(""));
		}
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Display updated successfully"));
}

void UQuestCompletionWidget::OnContinueButtonClicked()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Continue button clicked"));

	// Broadcast delegate
	OnContinueClicked.Broadcast();

	// Close widget
	CloseWidget();
}

void UQuestCompletionWidget::CloseWidget()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Closing widget"));

	// Restore game input mode
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Input mode restored to Game"));
	}

	// Remove from viewport
	RemoveFromParent();
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COMPLETION: Widget closed"));
}
