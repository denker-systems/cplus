// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestCollectable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UQuestCollectable : public UInterface
{
	GENERATED_BODY()
};

class CPLUS_API IQuestCollectable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	void OnCollectedForQuest(AActor* Collector);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	FName GetItemID() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	int32 GetItemCount() const;
};
