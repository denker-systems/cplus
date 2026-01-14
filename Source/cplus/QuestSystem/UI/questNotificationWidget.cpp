// Fill out your copyright notice in the Description page of Project Settings.

#include "questNotificationWidget.h"
#include "questUIData.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "QuestTask.h"

void UQuestNotificationWidget::ShowQuestStarted(UQuestDefinition* Quest)
{
	UE_LOG(LogTemp, Display, TEXT(">>> NOTIFICATION WIDGET: ShowQuestStarted called"));
	
	if (!Quest)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> NOTIFICATION WIDGET: Quest is NULL!"));
		return;
	}

	// Get quest title from UI data
	FText QuestTitle = Quest->UIData.QuestTitle.IsEmpty() 
		? FText::FromString(Quest->QuestID.ToString()) 
		: Quest->UIData.QuestTitle;
	
	UE_LOG(LogTemp, Display, TEXT(">>> NOTIFICATION WIDGET: Quest Title = %s"), *QuestTitle.ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> NOTIFICATION WIDGET: Calling Blueprint event OnQuestStarted"));
	
	// Call Blueprint event
	OnQuestStarted(QuestTitle);
	
	UE_LOG(LogTemp, Display, TEXT(">>> NOTIFICATION WIDGET: Blueprint event OnQuestStarted complete"));
}

void UQuestNotificationWidget::ShowQuestCompleted(UQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}

	// Get quest title from UI data
	FText QuestTitle = Quest->UIData.QuestTitle.IsEmpty() 
		? FText::FromString(Quest->QuestID.ToString()) 
		: Quest->UIData.QuestTitle;
	
	// Call Blueprint event
	OnQuestCompleted(QuestTitle);
}

void UQuestNotificationWidget::ShowQuestFailed(UQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}

	// Get quest title from UI data
	FText QuestTitle = Quest->UIData.QuestTitle.IsEmpty() 
		? FText::FromString(Quest->QuestID.ToString()) 
		: Quest->UIData.QuestTitle;
	
	// Call Blueprint event
	OnQuestFailed(QuestTitle);
}

void UQuestNotificationWidget::ShowObjectiveUpdated(UQuestDefinition* Quest, int32 ObjectiveIndex, int32 CurrentProgress, int32 TargetProgress)
{
	if (!Quest)
	{
		return;
	}

	// Get objective text from quest definition
	FText ObjectiveDescription = FText::FromString(TEXT("Objective Updated"));
	if (Quest->Objectives.IsValidIndex(ObjectiveIndex))
	{
		ObjectiveDescription = Quest->Objectives[ObjectiveIndex].ObjectiveDescription;
	}
	
	// Call Blueprint event
	OnObjectiveUpdated(ObjectiveDescription, CurrentProgress, TargetProgress);
}

void UQuestNotificationWidget::TrackQuest(UQuestDefinition* Quest, int32 CurrentObjectiveIndex)
{
	if (!Quest)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestNotificationWidget: Cannot track null quest"));
		return;
	}

	// Stop tracking previous quest if any
	if (bIsTracking)
	{
		UntrackQuest();
	}

	// Start tracking new quest
	TrackedQuest = Quest;
	TrackedObjectiveIndex = CurrentObjectiveIndex;
	bIsTracking = true;

	UE_LOG(LogTemp, Log, TEXT("QuestNotificationWidget: Now tracking quest '%s'"), *Quest->QuestID.ToString());

	// Show tracker container
	if (TrackerContainer)
	{
		TrackerContainer->SetVisibility(ESlateVisibility::Visible);
	}

	// Populate tracker content automatically
	PopulateTrackerContent();

	// Call Blueprint event for custom behavior (optional)
	OnQuestTracked(Quest, CurrentObjectiveIndex);
}

void UQuestNotificationWidget::UntrackQuest()
{
	if (!bIsTracking)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("QuestNotificationWidget: Stopped tracking quest"));

	// Hide tracker container
	if (TrackerContainer)
	{
		TrackerContainer->SetVisibility(ESlateVisibility::Hidden);
	}

	// Clear task container
	if (TasksContainer)
	{
		TasksContainer->ClearChildren();
	}

	// Clear tracking state
	TrackedQuest = nullptr;
	TrackedObjectiveIndex = 0;
	bIsTracking = false;

	// Call Blueprint event for custom behavior (optional)
	OnQuestUntracked();
}

void UQuestNotificationWidget::UpdateTrackedProgress()
{
	UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: UpdateTrackedProgress called"));
	
	if (!bIsTracking)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> TRACKER: Not tracking any quest (bIsTracking=false)"));
		return;
	}
	
	if (!TrackedQuest)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: TrackedQuest is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Updating progress for quest '%s'"), *TrackedQuest->QuestID.ToString());

	// Repopulate tracker content automatically
	PopulateTrackerContent();

	// Call Blueprint event for custom behavior (optional)
	OnTrackedProgressUpdated(TrackedQuest, TrackedObjectiveIndex);
}

void UQuestNotificationWidget::PopulateTrackerContent()
{
	UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: ========== PopulateTrackerContent START =========="));
	
	if (!TrackedQuest)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: TrackedQuest is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: TrackedQuest = %s"), *TrackedQuest->QuestID.ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: TrackedObjectiveIndex = %d"), TrackedObjectiveIndex);

	// Update quest title
	if (QuestTitleText)
	{
		FText TitleText = GetQuestTitleText();
		QuestTitleText->SetText(TitleText);
		UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Quest title set to '%s'"), *TitleText.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: QuestTitleText is NULL!"));
	}

	// Update objective text
	if (ObjectiveText)
	{
		FText ObjText = GetObjectiveText();
		ObjectiveText->SetText(ObjText);
		UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Objective text set to '%s'"), *ObjText.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: ObjectiveText is NULL!"));
	}

	// Clear and repopulate tasks
	if (TasksContainer)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Clearing TasksContainer"));
		TasksContainer->ClearChildren();

		// Get current objective
		if (TrackedQuest->Objectives.IsValidIndex(TrackedObjectiveIndex))
		{
			const FQuestObjective& CurrentObjective = TrackedQuest->Objectives[TrackedObjectiveIndex];
			UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Current objective has %d task(s)"), CurrentObjective.Tasks.Num());

			// Create entry for each task
			int32 TaskIndex = 0;
			for (UQuestTask* Task : CurrentObjective.Tasks)
			{
				if (!Task)
				{
					UE_LOG(LogTemp, Warning, TEXT(">>> TRACKER: Task[%d] is NULL!"), TaskIndex);
					TaskIndex++;
					continue;
				}

				UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Creating entry for Task[%d]: %s"), TaskIndex, *Task->TaskDescription.ToString());
				UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Task progress: %d/%d"), Task->CurrentCount, Task->TargetCount);

				// Create horizontal box for task entry
				UHorizontalBox* TaskEntry = NewObject<UHorizontalBox>(this);
				if (!TaskEntry)
				{
					UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: Failed to create TaskEntry!"));
					TaskIndex++;
					continue;
				}

				// Bullet point
				UTextBlock* BulletText = NewObject<UTextBlock>(this);
				if (BulletText)
				{
					BulletText->SetText(FText::FromString(TEXT("• ")));
					BulletText->SetColorAndOpacity(FLinearColor::White);
					TaskEntry->AddChild(BulletText);
				}

				// Task type (Collect, Kill, etc.)
				UTextBlock* TaskTypeText = NewObject<UTextBlock>(this);
				if (TaskTypeText)
				{
					FText TypeName = Task->GetTaskTypeName();
					TaskTypeText->SetText(FText::Format(FText::FromString(TEXT("{0}: ")), TypeName));
					
					// Color code by type
					if (TypeName.ToString() == TEXT("Collect"))
						TaskTypeText->SetColorAndOpacity(FLinearColor(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan
					else if (TypeName.ToString() == TEXT("Kill"))
						TaskTypeText->SetColorAndOpacity(FLinearColor(1.0f, 0.2f, 0.2f, 1.0f)); // Red
					else if (TypeName.ToString() == TEXT("Interact"))
						TaskTypeText->SetColorAndOpacity(FLinearColor(1.0f, 0.6f, 0.0f, 1.0f)); // Orange
					else if (TypeName.ToString() == TEXT("Explore"))
						TaskTypeText->SetColorAndOpacity(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f)); // Green
					else if (TypeName.ToString() == TEXT("Talk"))
						TaskTypeText->SetColorAndOpacity(FLinearColor(0.8f, 0.4f, 1.0f, 1.0f)); // Purple
					else
						TaskTypeText->SetColorAndOpacity(FLinearColor::White);
					
					TaskEntry->AddChild(TaskTypeText);
				}

				// Task description
				UTextBlock* TaskDescText = NewObject<UTextBlock>(this);
				if (TaskDescText)
				{
					TaskDescText->SetText(Task->TaskDescription);
					TaskDescText->SetColorAndOpacity(FLinearColor::White);
					TaskEntry->AddChild(TaskDescText);
				}

				// Progress counter
				UTextBlock* ProgressText = NewObject<UTextBlock>(this);
				if (ProgressText)
				{
					FText ProgressString = FText::Format(
						FText::FromString(TEXT(" ({0})")), 
						Task->GetProgressText(false)
					);
					ProgressText->SetText(ProgressString);
					ProgressText->SetColorAndOpacity(GetTaskColor(Task));
					TaskEntry->AddChild(ProgressText);
				}

				// Add task entry to container
				TasksContainer->AddChild(TaskEntry);
				UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: Task[%d] entry added to container"), TaskIndex);
				TaskIndex++;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: Invalid objective index %d (Objectives.Num=%d)"), TrackedObjectiveIndex, TrackedQuest->Objectives.Num());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> TRACKER: TasksContainer is NULL!"));
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> TRACKER: ========== PopulateTrackerContent END =========="));
}

FText UQuestNotificationWidget::GetQuestTitleText() const
{
	if (!TrackedQuest)
	{
		return FText::GetEmpty();
	}

	return FText::Format(
		FText::FromString(TEXT("Quest: {0}")),
		TrackedQuest->UIData.QuestTitle.IsEmpty() 
			? FText::FromName(TrackedQuest->QuestID) 
			: TrackedQuest->UIData.QuestTitle
	);
}

FText UQuestNotificationWidget::GetObjectiveText() const
{
	if (!TrackedQuest || !TrackedQuest->Objectives.IsValidIndex(TrackedObjectiveIndex))
	{
		return FText::GetEmpty();
	}

	int32 TotalObjectives = TrackedQuest->Objectives.Num();
	const FQuestObjective& CurrentObjective = TrackedQuest->Objectives[TrackedObjectiveIndex];

	return FText::Format(
		FText::FromString(TEXT("Objective {0}/{1}: {2}")),
		FText::AsNumber(TrackedObjectiveIndex + 1),
		FText::AsNumber(TotalObjectives),
		CurrentObjective.ObjectiveDescription
	);
}

FLinearColor UQuestNotificationWidget::GetTaskColor(UQuestTask* Task) const
{
	if (!Task)
	{
		return FLinearColor::White;
	}

	// Green if complete
	if (Task->IsComplete())
	{
		return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green
	}

	// Yellow if in progress
	if (Task->CurrentCount > 0)
	{
		return FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
	}

	// White if not started
	return FLinearColor::White;
}
