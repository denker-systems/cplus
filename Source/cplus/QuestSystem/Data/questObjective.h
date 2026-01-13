// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "questObjective.generated.h"

/**
 * Quest Objective Data
 * 
 * Defines a single objective within a quest.
 * Can contain multiple tasks that must be completed.
 */
USTRUCT(BlueprintType)
struct CPLUS_API FQuestObjective
{
	GENERATED_BODY()

	/** Display text for this objective shown in UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
	FText ObjectiveDescription;

	/** Tasks that must be completed for this objective */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Objective")
	TArray<TObjectPtr<UQuestTask>> Tasks;

	/** Is this objective optional? Quest can complete without it */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
	bool bIsOptional = false;

	/** Should this objective be hidden until previous objectives complete? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
	bool bIsHidden = false;

	/** Order index for displaying objectives */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
	int32 DisplayOrder = 0;

	/**
	 * Check if this objective is complete
	 * @return True if all required tasks are complete
	 */
	bool IsComplete() const
	{
		for (const TObjectPtr<UQuestTask>& Task : Tasks)
		{
			if (Task && !Task->IsComplete())
			{
				return false;
			}
		}
		return true;
	}
};
