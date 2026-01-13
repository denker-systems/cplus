// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDefinition.h"

FPrimaryAssetId UQuestDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Quests", GetFName());
}
