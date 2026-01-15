// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeaponSystem/Actors/WeaponPickup.h"
#include "WeaponSystem/Actors/BaseWeapon.h"
#include "WeaponSystem/Components/WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AWeaponPickup::AWeaponPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create interaction sphere
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(100.0f);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponPickup::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor || bIsCollected || !WeaponClass)
	{
		return;
	}

	// Find WeaponComponent on interactor
	UWeaponComponent* WeaponComp = Interactor->FindComponentByClass<UWeaponComponent>();
	if (!WeaponComp)
	{
		return;
	}

	// Check if already owns this weapon
	if (WeaponComp->FindWeaponOfType(WeaponClass))
	{
		// TODO: Could add ammo instead
		return;
	}

	// Add weapon to interactor
	WeaponComp->AddWeaponClass(WeaponClass);

	// Play pickup sound
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}

	// Call Blueprint event
	OnPickedUp(Interactor);

	// Mark as collected
	bIsCollected = true;

	// Hide or destroy
	if (bRespawns)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		// Schedule respawn
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimerHandle,
			this,
			&AWeaponPickup::Respawn,
			RespawnDelay,
			false
		);
	}
	else
	{
		Destroy();
	}
}

bool AWeaponPickup::CanInteract_Implementation(AActor* Interactor) const
{
	if (!Interactor || bIsCollected || !WeaponClass)
	{
		return false;
	}

	// Check if interactor has WeaponComponent
	UWeaponComponent* WeaponComp = Interactor->FindComponentByClass<UWeaponComponent>();
	if (!WeaponComp)
	{
		return false;
	}

	// Check if already owns this weapon
	if (WeaponComp->FindWeaponOfType(WeaponClass))
	{
		return false;
	}

	return true;
}

FText AWeaponPickup::GetInteractionText_Implementation() const
{
	return InteractionPrompt;
}

void AWeaponPickup::ShowPrompt_Implementation()
{
	// Override in Blueprint for UI prompt
}

void AWeaponPickup::HidePrompt_Implementation()
{
	// Override in Blueprint for UI prompt
}

void AWeaponPickup::Respawn()
{
	bIsCollected = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}
