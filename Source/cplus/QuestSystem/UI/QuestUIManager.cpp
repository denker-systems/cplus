// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestUIManager.h"
#include "questNotificationWidget.h"
#include "questJournalWidget.h"
#include "QuestSubSystem.h"
#include "Core/BaseGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UQuestUIManager::UQuestUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestUIManager::BeginPlay()
{
	Super::BeginPlay();

	// Get QuestSubsystem
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	}

	// Bind to quest events
	BindToQuestSubsystem();

	// Create notification widget
	CreateNotificationWidget();

	// Create journal widget (but don't show it yet)
	CreateJournalWidget();
}

void UQuestUIManager::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	UnbindFromQuestSubsystem();
	Super::EndPlay(EndPlayReason);
}

void UQuestUIManager::BindToQuestSubsystem()
{
	if (!QuestSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestUIManager: Cannot bind - QuestSubsystem is null"));
		return;
	}

	// Bind to quest events
	QuestSubsystem->OnQuestStarted.AddDynamic(this, &UQuestUIManager::HandleQuestStarted);
	QuestSubsystem->OnQuestCompleted.AddDynamic(this, &UQuestUIManager::HandleQuestCompleted);
	QuestSubsystem->OnQuestTaskUpdated.AddDynamic(this, &UQuestUIManager::HandleQuestTaskUpdated);

	UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Bound to QuestSubsystem delegates"));
}

void UQuestUIManager::UnbindFromQuestSubsystem()
{
	if (!QuestSubsystem)
	{
		return;
	}

	// Unbind from quest events
	QuestSubsystem->OnQuestStarted.RemoveDynamic(this, &UQuestUIManager::HandleQuestStarted);
	QuestSubsystem->OnQuestCompleted.RemoveDynamic(this, &UQuestUIManager::HandleQuestCompleted);
	QuestSubsystem->OnQuestTaskUpdated.RemoveDynamic(this, &UQuestUIManager::HandleQuestTaskUpdated);
}

void UQuestUIManager::CreateNotificationWidget()
{
	if (!NotificationWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestUIManager: NotificationWidgetClass not set"));
		return;
	}

	NotificationWidget = CreateWidget<UQuestNotificationWidget>(GetWorld(), NotificationWidgetClass);
	if (NotificationWidget)
	{
		NotificationWidget->AddToViewport(100); // Lower Z-order than journal
		
		// Check if Main Menu is active and hide if so
		if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
		{
			if (ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(GameMode))
			{
				if (BaseGameMode->IsMainMenuActive())
				{
					NotificationWidget->SetVisibility(ESlateVisibility::Hidden);
					UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Notification widget created and hidden (Main Menu active)"));
					return;
				}
			}
		}
		
		UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Notification widget created"));
	}
}

void UQuestUIManager::CreateJournalWidget()
{
	if (!JournalWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestUIManager: JournalWidgetClass is not set"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestUIManager: Cannot create journal widget - no PlayerController"));
		return;
	}

	JournalWidget = CreateWidget<UQuestJournalWidget>(PC, JournalWidgetClass);
	if (JournalWidget)
	{
		// Don't add to viewport yet - will be added when opened
		UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Journal widget created"));
	}
}

void UQuestUIManager::HandleQuestStarted(FName QuestID, UQuestDefinition* Quest)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: HandleQuestStarted called"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: QuestID = %s"), *QuestID.ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Quest = %s"), Quest ? *Quest->UIData.QuestTitle.ToString() : TEXT("NULL"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: bAutoShowQuestStarted = %s"), bAutoShowQuestStarted ? TEXT("true") : TEXT("false"));

	if (bAutoShowQuestStarted)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Calling ShowQuestStartedNotification"));
		ShowQuestStartedNotification(QuestID, Quest);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST UI MANAGER: Auto-show disabled, skipping notification"));
	}

	// Auto-track new quest if enabled
	if (NotificationWidget && NotificationWidget->bAutoTrackNewQuests)
	{
		UE_LOG(LogTemp, Log, TEXT(">>> QUEST UI MANAGER: Auto-tracking new quest"));
		NotificationWidget->TrackQuest(Quest, 0);
	}
}

void UQuestUIManager::HandleQuestCompleted(FName QuestID, const FQuestReward& Rewards)
{
	UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Quest completed - %s (XP:%d Gold:%d)"), 
		*QuestID.ToString(), Rewards.ExperiencePoints, Rewards.Gold);

	if (bAutoShowQuestCompleted)
	{
		ShowQuestCompletedNotification(QuestID, Rewards);
	}

	// Untrack quest if it's the one being tracked
	if (NotificationWidget && NotificationWidget->IsTrackingQuest())
	{
		UQuestDefinition* TrackedQuest = NotificationWidget->GetTrackedQuest();
		if (TrackedQuest && TrackedQuest->QuestID == QuestID)
		{
			UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Untracking completed quest"));
			NotificationWidget->UntrackQuest();
		}
	}
}

void UQuestUIManager::HandleQuestTaskUpdated(FName QuestID, int32 ObjectiveIndex, int32 TaskIndex)
{
	UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Quest task updated - %s [%d:%d]"), 
		*QuestID.ToString(), ObjectiveIndex, TaskIndex);

	if (bAutoShowObjectiveUpdated)
	{
		ShowObjectiveUpdatedNotification(QuestID, ObjectiveIndex, TaskIndex);
	}

	// Update tracked quest progress if this is the tracked quest
	if (NotificationWidget && NotificationWidget->IsTrackingQuest())
	{
		UQuestDefinition* TrackedQuest = NotificationWidget->GetTrackedQuest();
		if (TrackedQuest && TrackedQuest->QuestID == QuestID)
		{
			UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Updating tracked quest progress"));
			NotificationWidget->UpdateTrackedProgress();
		}
	}
}

void UQuestUIManager::ShowQuestStartedNotification(FName QuestID, UQuestDefinition* Quest)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: ShowQuestStartedNotification called"));
	
	if (!NotificationWidget)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UI MANAGER: NotificationWidget is NULL!"));
		return;
	}
	
	if (!Quest)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST UI MANAGER: Quest is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Calling NotificationWidget->ShowQuestStarted"));
	NotificationWidget->ShowQuestStarted(Quest);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: ShowQuestStarted call complete"));
}

void UQuestUIManager::ShowQuestCompletedNotification(FName QuestID, const FQuestReward& Rewards)
{
	if (!NotificationWidget || !QuestSubsystem)
	{
		return;
	}

	// Get quest definition from subsystem
	UQuestDefinition* Quest = QuestSubsystem->GetQuestDefinition(QuestID);
	if (Quest)
	{
		NotificationWidget->ShowQuestCompleted(Quest);
	}
}

void UQuestUIManager::ShowObjectiveUpdatedNotification(FName QuestID, int32 ObjectiveIndex, int32 TaskIndex)
{
	if (!NotificationWidget || !QuestSubsystem)
	{
		return;
	}

	// Get quest definition
	UQuestDefinition* Quest = QuestSubsystem->GetQuestDefinition(QuestID);
	if (!Quest || !Quest->Objectives.IsValidIndex(ObjectiveIndex))
	{
		return;
	}

	// Get task progress
	const FQuestObjective& Objective = Quest->Objectives[ObjectiveIndex];
	if (Objective.Tasks.IsValidIndex(TaskIndex))
	{
		UQuestTask* Task = Objective.Tasks[TaskIndex];
		if (Task)
		{
			NotificationWidget->ShowObjectiveUpdated(Quest, ObjectiveIndex, Task->CurrentCount, Task->TargetCount);
		}
	}
}

void UQuestUIManager::OpenQuestJournal()
{
	if (!JournalWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestUIManager: Cannot open journal - widget not created"));
		return;
	}

	if (bIsJournalOpen)
	{
		return; // Already open
	}

	// Add to viewport
	JournalWidget->AddToViewport(50); // Mid Z-order
	bIsJournalOpen = true;

	// Refresh journal data
	JournalWidget->RefreshJournal();

	// Set input mode to UI
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(JournalWidget->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}

	UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Journal opened"));
}

void UQuestUIManager::CloseQuestJournal()
{
	if (!JournalWidget)
	{
		return;
	}

	if (!bIsJournalOpen)
	{
		return; // Already closed
	}

	// Remove from viewport
	JournalWidget->RemoveFromParent();
	bIsJournalOpen = false;

	// Restore input mode to game
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	UE_LOG(LogTemp, Log, TEXT("QuestUIManager: Journal closed"));
}

void UQuestUIManager::ToggleQuestJournal()
{
	if (bIsJournalOpen)
	{
		CloseQuestJournal();
	}
	else
	{
		OpenQuestJournal();
	}
}

void UQuestUIManager::HideUI()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Hiding all quest UI"));

	// Hide notification widget
	if (NotificationWidget)
	{
		if (NotificationWidget->IsInViewport())
		{
			NotificationWidget->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Notification widget hidden"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Notification widget not in viewport"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Notification widget is null"));
	}

	// Hide journal widget
	if (JournalWidget)
	{
		if (JournalWidget->IsInViewport())
		{
			JournalWidget->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Journal widget hidden"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Journal widget not in viewport"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Journal widget is null"));
	}
}

void UQuestUIManager::ShowUI()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST UI MANAGER: Showing all quest UI"));

	// Show notification widget (if it exists)
	if (NotificationWidget && NotificationWidget->IsInViewport())
	{
		NotificationWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	// Show journal widget only if it was open before
	if (JournalWidget && JournalWidget->IsInViewport() && bIsJournalOpen)
	{
		JournalWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
