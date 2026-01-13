// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "GameplayTagContainer.h"
#include "QuestTask_Collect.generated.h"

/**
 * Quest Task: Collect Items
 * 
 * Tracks collection of specific items.
 * Can use either ItemID or ItemTag for flexible item identification.
 */
UCLASS()
class CPLUS_API UQuestTask_Collect : public UQuestTask
{
	GENERATED_BODY()

public:
	UQuestTask_Collect();

	/** Specific item ID to collect (e.g., "Item_GoldCoin") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	FName ItemID;

	/** GameplayTag for item category (e.g., Item.Type.Coin) - alternative to ItemID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	FGameplayTag ItemTag;

	/** Should items be consumed when collected for quest? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	bool bConsumeItems = false;

	/**
	 * Check if an item counts for this task
	 * @param CheckItemID The item ID to check
	 * @param ItemTags Tags from the item
	 * @return True if item counts for this task
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
	bool DoesItemCount(FName CheckItemID, const FGameplayTagContainer& ItemTags) const;
};
