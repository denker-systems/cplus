// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupComponent.h"
#include "InventoryComponent.h"
#include "QuestTargetComponent.h"
#include "QuestSubSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

UPickupComponent::UPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupComponent::BeginPlay()
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

		// Setup overlap detection if auto pickup
		if (bAutoPickup)
		{
			USphereComponent* SphereComp = Owner->FindComponentByClass<USphereComponent>();
			if (SphereComp)
			{
				SphereComp->OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnOverlapBegin);
			}
		}
	}
}

bool UPickupComponent::PickupItem(AActor* Collector)
{
	if (!CanBePickedUp(Collector))
	{
		return false;
	}

	// Add to inventory (this also notifies quest system via UInventoryComponent::NotifyQuestSystem)
	AddToInventory(Collector);

	// NOTE: Quest notification removed here - handled by InventoryComponent::AddItem() to avoid double-notify

	// Broadcast event
	OnItemPickedUp.Broadcast(Collector, ItemData);

	// Destroy or disable actor
	if (bDestroyOnPickup)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			Owner->Destroy();
		}
	}

	return true;
}

bool UPickupComponent::CanBePickedUp(AActor* Collector) const
{
	if (!Collector || !ItemData)
	{
		return false;
	}

	// Check if collector has inventory component
	UInventoryComponent* Inventory = Collector->FindComponentByClass<UInventoryComponent>();
	if (!Inventory)
	{
		return false;
	}

	// Check if inventory has space
	return Inventory->HasSpace(ItemData, ItemQuantity);
}

void UPickupComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                       bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAutoPickup || !OtherActor)
	{
		return;
	}

	// Check if other actor has inventory
	UInventoryComponent* Inventory = OtherActor->FindComponentByClass<UInventoryComponent>();
	if (Inventory)
	{
		PickupItem(OtherActor);
	}
}

void UPickupComponent::AddToInventory(AActor* Collector)
{
	if (!Collector || !ItemData)
	{
		return;
	}

	UInventoryComponent* Inventory = Collector->FindComponentByClass<UInventoryComponent>();
	if (Inventory)
	{
		Inventory->AddItem(ItemData, ItemQuantity);
	}
}

void UPickupComponent::NotifyQuestSystem(AActor* Collector)
{
	if (!ItemData || !QuestTarget)
	{
		return;
	}

	// Configure quest target tags from item
	QuestTarget->QuestTags = ItemData->ItemTags;

	// Notify via quest target component
	QuestTarget->OnCollected(Collector);
}

