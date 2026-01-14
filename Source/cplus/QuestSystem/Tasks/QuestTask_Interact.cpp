// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask_Interact.h"

UQuestTask_Interact::UQuestTask_Interact()
{
	TaskDescription = FText::FromString("Interact with Objects");
}

FText UQuestTask_Interact::GetTaskTypeName() const
{
	return FText::FromString("Interact");
}
