// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask_Kill.h"

UQuestTask_Kill::UQuestTask_Kill()
{
	TaskDescription = FText::FromString(TEXT("Kill enemies"));
}

bool UQuestTask_Kill::DoesEnemyCount(const FGameplayTagContainer& EnemyTags) const
{
	// If accepting any enemy, always return true
	if (bAcceptAnyEnemy)
	{
		return true;
	}

	// Check if enemy has the required tag
	if (EnemyTag.IsValid())
	{
		return EnemyTags.HasTag(EnemyTag);
	}

	// No valid tag specified, don't count
	return false;
}

FText UQuestTask_Kill::GetTaskTypeName() const
{
	return FText::FromString("Kill");
}
