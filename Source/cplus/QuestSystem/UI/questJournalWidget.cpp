// Fill out your copyright notice in the Description page of Project Settings.

#include "questJournalWidget.h"
#include "QuestSubSystem.h"
#include "Kismet/GameplayStatics.h"

void UQuestJournalWidget::RefreshJournal()
{
	OnQuestDataRefreshed();
}

TArray<FQuestProgressData> UQuestJournalWidget::GetActiveQuests() const
{
	TArray<FQuestProgressData> ActiveQuests;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return ActiveQuests;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		return ActiveQuests;
	}

	// Get active quests from subsystem
	// TODO: Implement GetActiveQuestProgress in QuestSubsystem
	
	return ActiveQuests;
}

TArray<FQuestProgressData> UQuestJournalWidget::GetCompletedQuests() const
{
	TArray<FQuestProgressData> CompletedQuests;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return CompletedQuests;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		return CompletedQuests;
	}

	// Get completed quests from subsystem
	// TODO: Implement GetCompletedQuestProgress in QuestSubsystem
	
	return CompletedQuests;
}

UQuestDefinition* UQuestJournalWidget::GetQuestDefinition(FName QuestID) const
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return nullptr;
	}

	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		return nullptr;
	}

	// Get quest definition from subsystem
	// TODO: Implement GetQuestDefinition in QuestSubsystem
	
	return nullptr;
}
