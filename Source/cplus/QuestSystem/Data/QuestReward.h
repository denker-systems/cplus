// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestReward.generated.h"

/**
 * Quest Reward Data
 * 
 * Defines rewards given upon quest completion.
 * Supports multiple reward types for flexible quest design.
 */
USTRUCT(BlueprintType)
struct CPLUS_API FQuestReward
{
	GENERATED_BODY()

	/** Experience points awarded */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	int32 ExperiencePoints = 0;

	/** Gold/currency awarded */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	int32 Gold = 0;

	/** Item IDs to give as rewards */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	TArray<FName> ItemRewards;

	/** Optional reputation gain with factions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	int32 ReputationGain = 0;

	/** Custom reward data for game-specific rewards */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	TMap<FName, int32> CustomRewards;
};
