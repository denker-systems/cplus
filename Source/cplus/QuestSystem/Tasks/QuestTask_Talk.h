// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "GameplayTagContainer.h"
#include "QuestTask_Talk.generated.h"

/**
 * Quest Task: Talk to NPC
 * 
 * Tracks conversations with specific NPCs.
 * Uses GameplayTags to identify valid NPCs to talk to.
 */
UCLASS()
class CPLUS_API UQuestTask_Talk : public UQuestTask
{
	GENERATED_BODY()

public:
	UQuestTask_Talk();

	/** GameplayTag to identify which NPC to talk to (e.g., NPC.Type.Merchant) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	FGameplayTag NPCTag;

	/** Specific NPC ID to talk to (alternative to NPCTag) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	FName NPCID;

	/**
	 * Check if an NPC with given tags/ID counts for this task
	 * @param CheckNPCID The NPC ID to check
	 * @param NPCTags Tags from the NPC
	 * @return True if NPC counts for this task
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
	bool DoesNPCCount(FName CheckNPCID, const FGameplayTagContainer& NPCTags) const;

	// UQuestTask interface
	virtual FText GetTaskTypeName() const override;
};
