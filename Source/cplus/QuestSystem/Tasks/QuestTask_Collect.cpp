// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask_Collect.h"

UQuestTask_Collect::UQuestTask_Collect()
{
	TaskDescription = FText::FromString(TEXT("Collect items"));
}

bool UQuestTask_Collect::DoesItemCount(FName CheckItemID, const FGameplayTagContainer& ItemTags) const
{
	// Check by ItemID first (exact match)
	if (ItemID != NAME_None && CheckItemID == ItemID)
	{
		return true;
	}

	// Check by ItemTag (category match)
	if (ItemTag.IsValid() && ItemTags.HasTag(ItemTag))
	{
		return true;
	}

	// Item doesn't match criteria
	return false;
}

FText UQuestTask_Collect::GetTaskTypeName() const
{
	return FText::FromString("Collect");
}
