// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestInteractable.h"
#include "WeaponPickup.generated.h"

class ABaseWeapon;
class USphereComponent;
class UStaticMeshComponent;

/**
 * Weapon Pickup Actor
 * 
 * Place in world for player to pick up weapons.
 * Implements IQuestInteractable for interaction system integration.
 */
UCLASS()
class CPLUS_API AWeaponPickup : public AActor, public IQuestInteractable
{
	GENERATED_BODY()
	
public:	
	AWeaponPickup();

	/** Visual mesh for the pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	/** Interaction sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> InteractionSphere;

	/** Weapon class to give on pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	/** Interaction prompt text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionPrompt = FText::FromString(TEXT("Press E to pick up weapon"));

	/** Should this pickup respawn after being collected? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bRespawns = false;

	/** Respawn delay in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "bRespawns"))
	float RespawnDelay = 30.0f;

	/** Pickup sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
	TObjectPtr<class USoundBase> PickupSound;

	// IQuestInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual FText GetInteractionText_Implementation() const override;
	virtual void ShowPrompt_Implementation() override;
	virtual void HidePrompt_Implementation() override;

protected:
	virtual void BeginPlay() override;

	/** Called when weapon is picked up */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnPickedUp(AActor* Picker);

	/** Respawn the pickup */
	UFUNCTION()
	void Respawn();

private:
	/** Has this pickup been collected? */
	bool bIsCollected = false;

	/** Timer handle for respawn */
	FTimerHandle RespawnTimerHandle;
};
