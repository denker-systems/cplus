// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask_Location.h"

UQuestTask_Location::UQuestTask_Location()
{
	TaskDescription = FText::FromString(TEXT("Reach location"));
	AcceptanceRadius = 500.0f;
}

bool UQuestTask_Location::IsLocationWithinRadius(const FVector& CheckLocation) const
{
	float Distance = FVector::Dist(CheckLocation, TargetLocation);
	return Distance <= AcceptanceRadius;
}

float UQuestTask_Location::GetDistanceToTarget(const FVector& FromLocation) const
{
	return FVector::Dist(FromLocation, TargetLocation);
}

FText UQuestTask_Location::GetTaskTypeName() const
{
	return FText::FromString("Explore");
}
