// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"
#include "InventoryComponent.h"
#include "QuestTargetComponent.h"
#include "GameFramework/Actor.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Find or create QuestTargetComponent
	AActor* Owner = GetOwner();
	if (Owner)
	{
		QuestTarget = Owner->FindComponentByClass<UQuestTargetComponent>();
		if (!QuestTarget)
		{
			QuestTarget = NewObject<UQuestTargetComponent>(Owner, UQuestTargetComponent::StaticClass());
			QuestTarget->RegisterComponent();
		}

		// Configure quest tags from interactable data
		if (InteractableData && QuestTarget)
		{
			QuestTarget->QuestTags = InteractableData->QuestTags;
		}
	}
}

void UInteractionComponent::Interact(AActor* Interactor)
{
	if (!CanInteract(Interactor))
	{
		return;
	}

	// Check requirements (items, etc.)
	if (!CheckRequirements(Interactor))
	{
		return;
	}

	// Mark as interacted
	bHasBeenInteracted = true;
	InteractionCount++;

	// Notify quest system
	NotifyQuestSystem(Interactor);

	// Broadcast event for Blueprint implementation
	OnInteracted.Broadcast(Interactor);
}

bool UInteractionComponent::CanInteract(AActor* Interactor) const
{
	if (!Interactor || !InteractableData)
	{
		return false;
	}

	// Check if already interacted and can't interact multiple times
	if (bHasBeenInteracted && !InteractableData->bCanInteractMultipleTimes)
	{
		return false;
	}

	// Check distance
	AActor* Owner = GetOwner();
	if (Owner)
	{
		float Distance = FVector::Dist(Owner->GetActorLocation(), Interactor->GetActorLocation());
		if (Distance > InteractableData->InteractionDistance)
		{
			return false;
		}
	}

	return true;
}

FText UInteractionComponent::GetInteractionPrompt() const
{
	if (InteractableData)
	{
		return InteractableData->InteractionPrompt;
	}
	return FText::FromString("Press E to interact");
}

bool UInteractionComponent::CheckRequirements(AActor* Interactor) const
{
	if (!InteractableData || !Interactor)
	{
		return true;
	}

	// Check if requires item
	if (InteractableData->bRequiresItem)
	{
		UInventoryComponent* Inventory = Interactor->FindComponentByClass<UInventoryComponent>();
		if (!Inventory)
		{
			return false;
		}

		// Check if has required item
		FInventoryItem RequiredItem = Inventory->GetItemByID(InteractableData->RequiredItemID);
		if (!RequiredItem.ItemData)
		{
			return false;
		}

		// Consume the item
		Inventory->RemoveItem(RequiredItem.ItemData, 1);
	}

	// Check required tags
	// TODO: Implement tag checking if needed

	return true;
}

void UInteractionComponent::NotifyQuestSystem(AActor* Interactor)
{
	if (!QuestTarget)
	{
		return;
	}

	// Notify via quest target component
	QuestTarget->OnInteracted(Interactor);
}

