// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask.h"

UQuestTask::UQuestTask()
{
	CurrentCount = 0;
	TargetCount = 1;
	bIsMandatory = true;
}

void UQuestTask::OnTaskStarted()
{
	CurrentCount = 0;
}

void UQuestTask::UpdateProgress(int32 Amount)
{
	CurrentCount = FMath::Clamp(CurrentCount + Amount, 0, TargetCount);
}

bool UQuestTask::IsComplete() const
{
	return CurrentCount >= TargetCount;
}

float UQuestTask::GetProgressPercent() const
{
	if (TargetCount <= 0)
	{
		return 0.0f;
	}
	return static_cast<float>(CurrentCount) / static_cast<float>(TargetCount);
}

FText UQuestTask::GetProgressText(bool bShowPercentage) const
{
	if (bShowPercentage)
	{
		int32 Percentage = FMath::RoundToInt(GetProgressPercent() * 100.0f);
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(Percentage));
	}
	else
	{
		return FText::Format(FText::FromString("{0}/{1}"), 
			FText::AsNumber(CurrentCount), 
			FText::AsNumber(TargetCount));
	}
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

