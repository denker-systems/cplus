#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestInteractable.h"
#include "GameplayTagContainer.h"
#include "QuestInteractableObject.generated.h"

/**
 * Quest Interactable Object
 * 
 * Object that can be interacted with for quest objectives.
 * Examples: Levers, chests, doors, NPCs, etc.
 * 
 * Usage:
 * - Place in level
 * - Set InteractionTag to match quest requirements
 * - Player presses E to interact
 */
UCLASS()
class CPLUS_API AQuestInteractableObject : public AActor, public IQuestInteractable
{
	GENERATED_BODY()
	
public:	
	AQuestInteractableObject();

	/** Visual mesh for the object */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	/** Interaction sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

	/** Text render component for interaction prompt */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* PromptText;

	/** Item ID for quest matching (used for Collect tasks) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName ItemID;

	/** Tag identifying this interactable for quests (used for Interact tasks) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FGameplayTag InteractionTag;

	/** Can this object be interacted with multiple times? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bCanInteractMultipleTimes = false;

	/** Interaction prompt text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText InteractionPrompt = FText::FromString(TEXT("Press E to interact"));

	/** Visual effect when interacted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|Effects")
	class UParticleSystem* InteractEffect;

	/** Sound when interacted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|Effects")
	class USoundBase* InteractSound;

	// IQuestInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual void ShowPrompt_Implementation() override;
	virtual void HidePrompt_Implementation() override;

protected:
	virtual void BeginPlay() override;

	/** Called when interaction happens */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void OnInteracted(AActor* Interactor);

private:
	/** Has this object been interacted with? */
	bool bHasBeenInteracted = false;
};
