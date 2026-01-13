// Fill out your copyright notice in the Description page of Project Settings.

#include "questNotificationWidget.h"
#include "questUIData.h"

void UQuestNotificationWidget::ShowQuestStarted(UQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}

	// Get quest title from UI data
	FText QuestTitle = FText::FromString(Quest->QuestID.ToString());
	
	// Call Blueprint event
	OnQuestStarted(QuestTitle);
}

void UQuestNotificationWidget::ShowQuestCompleted(UQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}

	// Get quest title from UI data
	FText QuestTitle = FText::FromString(Quest->QuestID.ToString());
	
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
	FText QuestTitle = FText::FromString(Quest->QuestID.ToString());
	
	// Call Blueprint event
	OnQuestFailed(QuestTitle);
}

void UQuestNotificationWidget::ShowObjectiveUpdated(UQuestDefinition* Quest, int32 ObjectiveIndex, int32 CurrentProgress, int32 TargetProgress)
{
	if (!Quest)
	{
		return;
	}

	// Get objective text
	FText ObjectiveText = FText::FromString(TEXT("Objective Updated"));
	
	// Call Blueprint event
	OnObjectiveUpdated(ObjectiveText, CurrentProgress, TargetProgress);
}
