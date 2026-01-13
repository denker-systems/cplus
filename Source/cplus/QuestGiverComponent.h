// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestDefinition.h"
#include "QuestGiverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestOffered, UQuestDefinition*, Quest, AActor*, Player);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPLUS_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UQuestGiverComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<TObjectPtr<UQuestDefinition>> AvailableQuests;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestOffered OnQuestOffered;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<UQuestDefinition*> GetAvailableQuests(AActor* Player);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OfferQuest(UQuestDefinition* Quest, AActor* Player);

protected:

	virtual void BeginPlay() override;
};
