// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestProgressWidget.h"
#include "QuestTask.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"

void UQuestProgressWidget::SetQuest(UQuestDefinition* Quest, int32 CurrentObjectiveIndex)
{
	if (!Quest)
	{
		return;
	}

	CurrentQuest = Quest;
	CurrentObjectiveIdx = CurrentObjectiveIndex;

	// Update quest title
	if (QuestTitleText)
	{
		QuestTitleText->SetText(Quest->UIData.QuestTitle);
	}

	// Update quest description
	if (QuestDescriptionText)
	{
		QuestDescriptionText->SetText(Quest->UIData.FullDescription);
	}

	// Refresh progress display
	RefreshProgress();

	// Call Blueprint event
	OnQuestSet(Quest);
}

void UQuestProgressWidget::RefreshProgress()
{
	if (!CurrentQuest)
	{
		return;
	}

	// Clear existing objective widgets
	if (ObjectivesContainer)
	{
		ObjectivesContainer->ClearChildren();
	}

	// Note: Actual widget creation should be done in Blueprint
	// This C++ class provides the data and logic
	// Blueprint will create the visual representation

	// Call Blueprint event
	OnProgressRefreshed();
}

FText UQuestProgressWidget::GetTaskProgressText(UQuestTask* Task)
{
	if (!Task)
	{
		return FText::FromString("Invalid Task");
	}

	return Task->GetDetailedProgressText();
}

FLinearColor UQuestProgressWidget::GetTaskColor(UQuestTask* Task)
{
	if (!Task)
	{
		return FLinearColor::White;
	}

	// DEPRECATED: Cannot determine progress without quest context
	// Use QuestSubsystem::GetTaskProgress() instead
	// Return white as default
	return FLinearColor::White;
}

FText UQuestProgressWidget::GetTaskTypeIcon(UQuestTask* Task)
{
	if (!Task)
	{
		return FText::FromString("?");
	}

	return Task->GetTaskTypeName();
}
