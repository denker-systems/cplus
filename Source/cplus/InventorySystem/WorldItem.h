// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCollectable.h"
#include "QuestInteractable.h"
#include "WorldItem.generated.h"

/**
 * World Item Actor
 * 
 * Physical representation of item in world.
 * Can be picked up and added to inventory.
 */
UCLASS()
class CPLUS_API AWorldItem : public AActor, public IQuestCollectable, public IQuestInteractable
{
	GENERATED_BODY()
	
public:
	AWorldItem();

	// ===== COMPONENTS =====
	
	/** Visual mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	/** Collision for pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> CollisionComponent;

	/** Pickup functionality */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPickupComponent> PickupComponent;

	// ===== SETTINGS =====
	
	/** Is this a pickup item? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsPickupItem = true;

protected:
	virtual void BeginPlay() override;

	// IQuestCollectable interface
	virtual void OnCollectedForQuest_Implementation(AActor* Collector) override;
	virtual FName GetItemID_Implementation() const override;
	virtual int32 GetItemCount_Implementation() const override;

	// IQuestInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual FText GetInteractionText_Implementation() const override;
};
