// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableDefinition.h"
#include "QuestInteractable.h"
#include "InteractionComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, Interactor);


/**
 * Interaction Component
 * 
 * Handles interaction logic for objects.
 * Attach to AInteractableObject actors.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	// ===== INTERACTABLE DATA =====
	
	/** Interactable definition reference */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInteractableDefinition> InteractableData;

	// ===== RUNTIME STATE =====
	
	/** Has been interacted with? */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	bool bHasBeenInteracted = false;

	/** Interaction count */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	int32 InteractionCount = 0;

	// ===== QUEST INTEGRATION =====
	
	/** Quest target component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<class UQuestTargetComponent> QuestTarget;

	// ===== EVENTS =====
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteracted OnInteracted;

	// ===== API =====
	
	/**
	 * Perform interaction
	 * @param Interactor Actor performing interaction
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);

	/**
	 * Check if can interact
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;

	/**
	 * Get interaction prompt text
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	FText GetInteractionPrompt() const;

protected:
	virtual void BeginPlay() override;

	bool CheckRequirements(AActor* Interactor) const;
	void NotifyQuestSystem(AActor* Interactor);
};
