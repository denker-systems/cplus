// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiverComponent.h"

// Sets default values for this component's properties
UQuestGiverComponent::UQuestGiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UQuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();
}

TArray<UQuestDefinition*> UQuestGiverComponent::GetAvailableQuests(AActor* Player)
{
	TArray<UQuestDefinition*> Result;
	for (UQuestDefinition* Quest : AvailableQuests)
	{
		if (Quest)
		{
			Result.Add(Quest);
		}
	}
	return Result;
}

void UQuestGiverComponent::OfferQuest(UQuestDefinition* Quest, AActor* Player)
{
	if (Quest && Player)
	{
		OnQuestOffered.Broadcast(Quest, Player);
	}
}
