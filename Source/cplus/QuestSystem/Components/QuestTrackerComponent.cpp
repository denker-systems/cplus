// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTrackerComponent.h"
#include "QuestSubSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQuestTrackerComponent::UQuestTrackerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UQuestTrackerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestTrackerComponent::AcceptQuest(UQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}

	// Add to local tracker
	ActiveQuestIDs.AddUnique(Quest->QuestID);
	OnQuestAccepted.Broadcast(Quest->QuestID, Quest);

	// Also add to QuestSubsystem so notifications work
	UQuestSubSystem* QuestSub = GetQuestSubsystem();
	if (QuestSub)
	{
		QuestSub->AcceptQuest(Quest);
	}
}

void UQuestTrackerComponent::AbandonQuest(FName QuestID)
{
	ActiveQuestIDs.Remove(QuestID);
}

TArray<FName> UQuestTrackerComponent::GetActiveQuestIDs() const
{
	return ActiveQuestIDs;
}

UQuestSubSystem* UQuestTrackerComponent::GetQuestSubsystem() const
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UQuestSubSystem>();
}
