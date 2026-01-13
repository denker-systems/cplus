// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "QuestTask_Location.generated.h"

UCLASS()
class CPLUS_API UQuestTask_Location : public UQuestTask
{
	GENERATED_BODY()

public:

	UQuestTask_Location();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	float AcceptanceRadius = 500.0f;
};
