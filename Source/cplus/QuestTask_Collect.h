// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "QuestTask_Collect.generated.h"

UCLASS()
class CPLUS_API UQuestTask_Collect : public UQuestTask
{
	GENERATED_BODY()

public:

	UQuestTask_Collect();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName ItemID;
};
