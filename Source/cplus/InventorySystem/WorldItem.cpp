// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldItem.h"
#include "PickupComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

AWorldItem::AWorldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(100.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupComponent = CreateDefaultSubobject<UPickupComponent>(TEXT("PickupComponent"));
}

void AWorldItem::BeginPlay()
{
	Super::BeginPlay();

	// Configure mesh from item data
	if (PickupComponent && PickupComponent->ItemData && PickupComponent->ItemData->WorldMesh)
	{
		MeshComponent->SetStaticMesh(PickupComponent->ItemData->WorldMesh);
		MeshComponent->SetWorldScale3D(PickupComponent->ItemData->WorldMeshScale);
	}
}

// IQuestCollectable interface
void AWorldItem::OnCollectedForQuest_Implementation(AActor* Collector)
{
	if (PickupComponent)
	{
		PickupComponent->PickupItem(Collector);
	}
}

FName AWorldItem::GetItemID_Implementation() const
{
	if (PickupComponent && PickupComponent->ItemData)
	{
		return PickupComponent->ItemData->ItemID;
	}
	return NAME_None;
}

int32 AWorldItem::GetItemCount_Implementation() const
{
	if (PickupComponent)
	{
		return PickupComponent->ItemQuantity;
	}
	return 0;
}

// IQuestInteractable interface
void AWorldItem::Interact_Implementation(AActor* Interactor)
{
	if (PickupComponent)
	{
		PickupComponent->PickupItem(Interactor);
	}
}

bool AWorldItem::CanInteract_Implementation(AActor* Interactor) const
{
	if (PickupComponent)
	{
		return PickupComponent->CanBePickedUp(Interactor);
	}
	return false;
}

FText AWorldItem::GetInteractionText_Implementation() const
{
	if (PickupComponent && PickupComponent->ItemData)
	{
		return FText::Format(FText::FromString("Press E to pickup {0}"), 
		                     PickupComponent->ItemData->ItemName);
	}
	return FText::FromString("Press E to pickup");
}

