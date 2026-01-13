// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InteractableDefinition.generated.h"

/**
 * Interactable Type Enumeration
 */
UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	Generic     UMETA(DisplayName = "Generic"),
	Door        UMETA(DisplayName = "Door"),
	Chest       UMETA(DisplayName = "Chest"),
	Lever       UMETA(DisplayName = "Lever"),
	Button      UMETA(DisplayName = "Button"),
	QuestObject UMETA(DisplayName = "Quest Object")
};

/**
 * Interactable Definition DataAsset
 * 
 * Defines properties of interactable objects (doors, chests, levers, etc.)
 */
UCLASS(BlueprintType)
class CPLUS_API UInteractableDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Unique interactable identifier */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Identity")
	FName InteractableID;

	/** Type of interactable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Type")
	EInteractableType InteractableType;

	// ===== UI DATA =====
	
	/** Prompt shown to player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|UI")
	FText InteractionPrompt;

	/** Icon for interaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|UI")
	TObjectPtr<UTexture2D> InteractionIcon;

	// ===== INTERACTION SETTINGS =====
	
	/** Can be interacted with multiple times? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Settings")
	bool bCanInteractMultipleTimes = false;

	/** Distance player must be within */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Settings")
	float InteractionDistance = 200.0f;

	/** Duration to hold interact button (0 = instant) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Settings")
	float InteractionDuration = 0.0f;

	// ===== REQUIREMENTS =====
	
	/** Requires specific item to interact? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Requirements")
	bool bRequiresItem = false;

	/** Required item ID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Requirements", meta = (EditCondition = "bRequiresItem"))
	FName RequiredItemID;

	/** Required gameplay tags */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Requirements")
	FGameplayTagContainer RequiredTags;

	// ===== QUEST INTEGRATION =====
	
	/** Tags for quest matching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Quest")
	FGameplayTagContainer QuestTags;

	// ===== FEEDBACK =====
	
	/** Sound played on interaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Feedback")
	TObjectPtr<USoundBase> InteractionSound;

	/** Particle effect on interaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Feedback")
	TObjectPtr<UParticleSystem> InteractionEffect;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Interactables", GetFName());
	}
};
