// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask.h"

UQuestTask::UQuestTask()
{
	TargetCount = 1;
	bIsMandatory = true;
}

void UQuestTask::OnTaskStarted()
{
	// Task started - progress tracked in FActiveQuest.TaskProgress
}

void UQuestTask::UpdateProgress(int32 Amount)
{
	// DEPRECATED: Progress now tracked in FActiveQuest.TaskProgress
	// Use QuestSubsystem::UpdateTaskProgress() instead
	UE_LOG(LogTemp, Warning, TEXT("QuestTask::UpdateProgress is deprecated - use QuestSubsystem"));
}

bool UQuestTask::IsComplete() const
{
	// DEPRECATED: Use QuestSubsystem::IsTaskComplete() instead
	UE_LOG(LogTemp, Warning, TEXT("QuestTask::IsComplete is deprecated - use QuestSubsystem"));
	return false;
}

float UQuestTask::GetProgressPercent() const
{
	// DEPRECATED: Cannot determine progress without FActiveQuest context
	// Use QuestSubsystem::GetTaskProgress() instead
	return 0.0f;
}

FText UQuestTask::GetProgressText(bool bShowPercentage) const
{
	// DEPRECATED: Cannot determine progress without FActiveQuest context
	// Use QuestSubsystem::GetTaskProgress() instead
	return FText::FromString(TEXT("0/0"));
}

FText UQuestTask::GetTaskTypeName() const
{
	return FText::FromString("Task");
}

FText UQuestTask::GetDetailedProgressText() const
{
	// Format: "TaskType: Progress - Description"
	// Example: "Collect: 3/10 - Collect Gold Coins"
	return FText::Format(FText::FromString("{0}: {1} - {2}"),
		GetTaskTypeName(),
		GetProgressText(false),
		TaskDescription);
}

