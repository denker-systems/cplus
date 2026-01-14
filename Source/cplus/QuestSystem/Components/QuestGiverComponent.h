// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestDefinition.h"
#include "QuestGiverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestOffered, UQuestDefinition*, Quest, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestTurnedIn, FName, QuestID, UQuestDefinition*, Quest, AActor*, Player);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPLUS_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UQuestGiverComponent();

	/** Manually added quests */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<TObjectPtr<UQuestDefinition>> AvailableQuests;

	/** Auto-load quests from this folder path (e.g. /Game/Plugin/Quests/TestQuests) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Auto-Load")
	FString AutoLoadQuestFolder = TEXT("/Game/Plugin/Quests/TestQuests");

	/** Enable auto-loading quests from folder */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Auto-Load")
	bool bAutoLoadQuests = true;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestOffered OnQuestOffered;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestTurnedIn OnQuestTurnedIn;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<UQuestDefinition*> GetAvailableQuests(AActor* Player);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OfferQuest(UQuestDefinition* Quest, AActor* Player);

	/**
	 * Handle interaction with player - automatically checks for completed quests first,
	 * then offers new quests if no completed quests to turn in.
	 * This is the main function to call from NPC interaction - handles everything in C++!
	 * 
	 * @param Player The player interacting with this NPC
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void HandleInteraction(AActor* Player);

	/**
	 * Get completed quests that can be turned in to this NPC
	 * @param Player The player to check
	 * @return Array of completed quests that this NPC can accept
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<UQuestDefinition*> GetCompletedQuests(AActor* Player);

	/**
	 * Turn in a completed quest to this NPC
	 * @param QuestID The quest to turn in
	 * @param Player The player turning in the quest
	 * @return True if quest was successfully turned in
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool TurnInQuest(FName QuestID, AActor* Player);

	/** Load all quests from specified folder */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void LoadQuestsFromFolder();

protected:

	virtual void BeginPlay() override;
};
