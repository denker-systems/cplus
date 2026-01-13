// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDefinition.h"
#include "QuestCollectable.h"
#include "PickupComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUp, AActor*, Collector, UItemDefinition*, Item);


/**
 * Pickup Component
 * 
 * Handles picking up items from world to inventory.
 * Attach to AWorldItem actors.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPickupComponent();

	// ===== PICKUP DATA =====
	
	/** Item definition reference */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UItemDefinition> ItemData;

	/** Quantity of items */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	int32 ItemQuantity = 1;

	// ===== PICKUP SETTINGS =====
	
	/** Auto pickup on overlap? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	bool bAutoPickup = true;

	/** Pickup detection radius */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	float PickupRadius = 100.0f;

	/** Destroy actor on pickup? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	bool bDestroyOnPickup = true;

	// ===== QUEST INTEGRATION =====
	
	/** Quest target component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<class UQuestTargetComponent> QuestTarget;

	// ===== EVENTS =====
	
	UPROPERTY(BlueprintAssignable, Category = "Pickup")
	FOnItemPickedUp OnItemPickedUp;

	// ===== API =====
	
	/**
	 * Attempt to pickup item
	 * @param Collector Actor trying to pickup
	 * @return True if successfully picked up
	 */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	bool PickupItem(AActor* Collector);

	/**
	 * Check if can be picked up
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pickup")
	bool CanBePickedUp(AActor* Collector) const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);

	void AddToInventory(AActor* Collector);
	void NotifyQuestSystem(AActor* Collector);
};
