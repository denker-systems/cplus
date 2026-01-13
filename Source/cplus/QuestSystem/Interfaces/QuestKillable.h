// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "QuestKillable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UQuestKillable : public UInterface
{
	GENERATED_BODY()
};

class CPLUS_API IQuestKillable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	void OnKilledForQuest(AActor* Killer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	FGameplayTagContainer GetKillTags() const;
};
