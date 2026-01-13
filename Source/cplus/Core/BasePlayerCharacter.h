#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BasePlayerCharacter.generated.h"

/**
 * Base Player Character
 * 
 * Base class for all player-controlled characters
 * Provides: Input, Camera, Quest Tracking, Inventory, Interaction
 */
UCLASS(Abstract)
class CPLUS_API ABasePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABasePlayerCharacter();

protected:
	virtual void BeginPlay() override;

	/** Quest tracker component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UQuestTrackerComponent* QuestTracker;

	/** Inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* Inventory;

	/** Interaction component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* InteractionComponent;

public:
	/** Get quest tracker */
	UFUNCTION(BlueprintPure, Category = "Player")
	UQuestTrackerComponent* GetQuestTracker() const { return QuestTracker; }

	/** Get inventory */
	UFUNCTION(BlueprintPure, Category = "Player")
	class UInventoryComponent* GetInventory() const { return Inventory; }

	/** Get interaction component */
	UFUNCTION(BlueprintPure, Category = "Player")
	UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
};
