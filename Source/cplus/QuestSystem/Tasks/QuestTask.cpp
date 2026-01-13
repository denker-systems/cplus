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

