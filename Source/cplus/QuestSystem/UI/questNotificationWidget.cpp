// Fill out your copyright notice in the Description page of Project Settings.

#include "questNotificationWidget.h"
#include "questUIData.h"

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
	FText ObjectiveText = FText::FromString(TEXT("Objective Updated"));
	if (Quest->Objectives.IsValidIndex(ObjectiveIndex))
	{
		ObjectiveText = Quest->Objectives[ObjectiveIndex].ObjectiveDescription;
	}
	
	// Call Blueprint event
	OnObjectiveUpdated(ObjectiveText, CurrentProgress, TargetProgress);
}
