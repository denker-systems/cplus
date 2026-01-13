// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"
#include "InteractionComponent.h"
#include "Components/StaticMeshComponent.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
}

// IQuestInteractable interface
void AInteractableObject::Interact_Implementation(AActor* Interactor)
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact(Interactor);
	}
}

bool AInteractableObject::CanInteract_Implementation(AActor* Interactor) const
{
	if (InteractionComponent)
	{
		return InteractionComponent->CanInteract(Interactor);
	}
	return false;
}

FText AInteractableObject::GetInteractionText_Implementation() const
{
	if (InteractionComponent)
	{
		return InteractionComponent->GetInteractionPrompt();
	}
	return FText::FromString("Press E to interact");
}

