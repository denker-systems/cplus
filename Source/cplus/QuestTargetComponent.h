// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "QuestTargetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPLUS_API UQuestTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UQuestTargetComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FGameplayTagContainer QuestTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName TargetID;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OnKilled(AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OnInteracted(AActor* Interactor);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OnCollected(AActor* Collector);

protected:

	virtual void BeginPlay() override;

	void NotifyQuestSystem(AActor* Instigator);
};
