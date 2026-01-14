// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestTask_Talk.h"

UQuestTask_Talk::UQuestTask_Talk()
{
	TaskDescription = FText::FromString(TEXT("Talk to NPC"));
}

bool UQuestTask_Talk::DoesNPCCount(FName CheckNPCID, const FGameplayTagContainer& NPCTags) const
{
	// Check by NPCID first (exact match)
	if (NPCID != NAME_None && CheckNPCID == NPCID)
	{
		return true;
	}

	// Check by NPCTag (category match)
	if (NPCTag.IsValid() && NPCTags.HasTag(NPCTag))
	{
		return true;
	}

	// NPC doesn't match criteria
	return false;
}

FText UQuestTask_Talk::GetTaskTypeName() const
{
	return FText::FromString("Talk");
}
