// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestInteractable.h"
#include "InteractableObject.generated.h"

/**
 * Interactable Object Actor
 * 
 * Base class for doors, chests, levers, etc.
 * Implements quest integration automatically.
 */
UCLASS()
class CPLUS_API AInteractableObject : public AActor, public IQuestInteractable
{
	GENERATED_BODY()
	
public:
	AInteractableObject();

	// ===== COMPONENTS =====
	
	/** Visual mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	/** Interaction functionality */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UInteractionComponent> InteractionComponent;

protected:
	virtual void BeginPlay() override;

	// IQuestInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual FText GetInteractionText_Implementation() const override;
};
