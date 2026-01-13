// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "GameplayTagContainer.h"
#include "QuestTask_Kill.generated.h"

/**
 * Quest Task: Kill Enemies
 * 
 * Tracks kills of specific enemy types.
 * Uses GameplayTags to identify valid targets.
 */
UCLASS()
class CPLUS_API UQuestTask_Kill : public UQuestTask
{
	GENERATED_BODY()

public:
	UQuestTask_Kill();

	/** GameplayTag to identify which enemies count for this task (e.g., Enemy.Type.Bandit) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	FGameplayTag EnemyTag;

	/** Should any enemy count, regardless of tag? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	bool bAcceptAnyEnemy = false;

	/**
	 * Check if an enemy with given tags counts for this task
	 * @param EnemyTags Tags from the killed enemy
	 * @return True if enemy counts for this task
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest Task")
	bool DoesEnemyCount(const FGameplayTagContainer& EnemyTags) const;
};
