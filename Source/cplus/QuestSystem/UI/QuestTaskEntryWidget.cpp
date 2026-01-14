// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestTaskEntryWidget.h"
#include "QuestTask.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UQuestTaskEntryWidget::SetTask(UQuestTask* Task)
{
	if (!Task)
	{
		return;
	}

	CurrentTask = Task;
	RefreshTask();

	// Call Blueprint event
	OnTaskSet(Task);
}

void UQuestTaskEntryWidget::RefreshTask()
{
	if (!CurrentTask)
	{
		return;
	}

	// Update task type text
	if (TaskTypeText)
	{
		TaskTypeText->SetText(CurrentTask->GetTaskTypeName());
	}

	// Update task description
	if (TaskDescriptionText)
	{
		TaskDescriptionText->SetText(CurrentTask->TaskDescription);
	}

	// Update progress counter
	if (ProgressCounterText)
	{
		ProgressCounterText->SetText(CurrentTask->GetProgressText(false));
	}

	// Update progress bar
	if (ProgressBar)
	{
		float Percent = CurrentTask->GetProgressPercent();
		ProgressBar->SetPercent(Percent);

		// Color based on completion
		// DEPRECATED: Cannot determine progress without quest context
		// Default to gray color
		FLinearColor BarColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		ProgressBar->SetFillColorAndOpacity(BarColor);
	}

	// Call Blueprint event
	OnTaskRefreshed();
}
