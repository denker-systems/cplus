// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UQuestInteractable : public UInterface
{
	GENERATED_BODY()
};

class CPLUS_API IQuestInteractable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	void Interact(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	bool CanInteract(AActor* Interactor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	FText GetInteractionText() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	void ShowPrompt();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	void HidePrompt();
};
