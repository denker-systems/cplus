// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTargetComponent.h"
#include "QuestSubSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQuestTargetComponent::UQuestTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UQuestTargetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestTargetComponent::OnKilled(AActor* Killer)
{
	NotifyQuestSystem(Killer);
}

void UQuestTargetComponent::OnInteracted(AActor* Interactor)
{
	NotifyQuestSystem(Interactor);
}

void UQuestTargetComponent::OnCollected(AActor* Collector)
{
	NotifyQuestSystem(Collector);
}

void UQuestTargetComponent::NotifyQuestSystem(AActor* Instigator)
{
	if (!Instigator || QuestTags.IsEmpty())
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance)
	{
		return;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (QuestSubsystem)
	{
		QuestSubsystem->NotifyQuestEvent(TargetID, QuestTags, Instigator);
	}
}
