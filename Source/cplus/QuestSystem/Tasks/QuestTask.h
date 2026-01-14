// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "QuestTask.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew)
class CPLUS_API UQuestTask : public UObject
{
	GENERATED_BODY()

public:

	UQuestTask();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName TaskID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText TaskDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 TargetCount = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bIsMandatory = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FGameplayTagContainer TaskTags;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	virtual void OnTaskStarted();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	virtual void UpdateProgress(int32 Amount = 1);

	UFUNCTION(BlueprintPure, Category = "Quest")
	virtual bool IsComplete() const;

	UFUNCTION(BlueprintPure, Category = "Quest")
	float GetProgressPercent() const;

	/**
	 * Get progress as formatted text (e.g., "3/10", "50%")
	 * @param bShowPercentage If true, shows percentage instead of fraction
	 * @return Formatted progress text
	 */
	UFUNCTION(BlueprintPure, Category = "Quest")
	FText GetProgressText(bool bShowPercentage = false) const;

	/**
	 * Get task type name for UI display (e.g., "Collect", "Kill", "Interact")
	 * Override in subclasses to provide specific type names
	 */
	UFUNCTION(BlueprintPure, Category = "Quest")
	virtual FText GetTaskTypeName() const;

	/**
	 * Get detailed progress description including task type and progress
	 * (e.g., "Collect: 3/10 Gold Coins")
	 */
	UFUNCTION(BlueprintPure, Category = "Quest")
	FText GetDetailedProgressText() const;
};
