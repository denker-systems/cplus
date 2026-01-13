// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDefinition.h"

bool FQuestObjective::IsComplete() const
{
	int32 CompletedCount = 0;
	int32 MandatoryCount = 0;

	for (const UQuestTask* Task : Tasks)
	{
		if (!Task) continue;

		if (Task->bIsMandatory)
		{
			MandatoryCount++;
			if (Task->IsComplete())
			{
				CompletedCount++;
			}
		}
	}

	switch (CompletionRule)
	{
		case EQuestCompletionRule::All:
			return CompletedCount >= MandatoryCount;

		case EQuestCompletionRule::Any:
			return CompletedCount > 0;

		case EQuestCompletionRule::NOfM:
			return CompletedCount >= RequiredTaskCount;

		default:
			return false;
	}
}

FPrimaryAssetId UQuestDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Quest", QuestID);
}

