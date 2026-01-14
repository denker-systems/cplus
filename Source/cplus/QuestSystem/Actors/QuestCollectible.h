#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestInteractable.h"
#include "GameplayTagContainer.h"
#include "QuestCollectible.generated.h"

/**
 * Quest Collectible Actor
 * 
 * Pickup item that can be collected for quest objectives.
 * Notifies QuestSubsystem when collected.
 * 
 * Usage:
 * - Place in level
 * - Set ItemID or ItemTags to match quest requirements
 * - Player walks over to collect
 */
UCLASS()
class CPLUS_API AQuestCollectible : public AActor, public IQuestInteractable
{
	GENERATED_BODY()
	
public:	
	AQuestCollectible();

	/** Visual mesh for the collectible */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	/** Interaction sphere for pickup detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

	/** Text render component for interaction prompt */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* PromptText;

	/** Item ID for quest matching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName ItemID;

	/** Item tags for quest matching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FGameplayTagContainer ItemTags;

	/** Should this item respawn after being collected? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bRespawns = false;

	/** Respawn delay in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (EditCondition = "bRespawns"))
	float RespawnDelay = 30.0f;

	/** Interaction prompt text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText InteractionPrompt = FText::FromString(TEXT("Press E to collect"));

	/** Visual effect when collected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|Effects")
	class UParticleSystem* CollectEffect;

	/** Sound when collected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|Effects")
	class USoundBase* CollectSound;

	// IQuestInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual void ShowPrompt_Implementation() override;
	virtual void HidePrompt_Implementation() override;

protected:
	virtual void BeginPlay() override;

	/** Collect this item */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void Collect(AActor* Collector);

	/** Respawn the collectible */
	UFUNCTION()
	void Respawn();

	/** Called when interaction happens */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void OnCollected(AActor* Collector);

private:
	/** Is this collectible currently collected? */
	bool bIsCollected = false;

	/** Timer handle for respawn */
	FTimerHandle RespawnTimerHandle;
};
