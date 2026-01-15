#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameplayTagContainer.h"
#include "QuestTriggerVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTriggerActivated, AActor*, TriggeringActor);

/**
 * Quest Trigger Volume
 * 
 * Invisible trigger box for location-based quest objectives
 */
UCLASS()
class CPLUS_API AQuestTriggerVolume : public AActor
{
	GENERATED_BODY()

public:
	AQuestTriggerVolume();

	/** Event when player enters trigger */
	UPROPERTY(BlueprintAssignable, Category = "Quest Trigger")
	FOnQuestTriggerActivated OnTriggerActivated;

protected:
	virtual void BeginPlay() override;

	/** Trigger box component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Trigger")
	UBoxComponent* TriggerBox;

	/** Tag identifying this location for quests (e.g., Quest.Objective.Location) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Trigger")
	FGameplayTag LocationTag;

	/** Quest ID this trigger is associated with (legacy - use LocationTag instead) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Trigger")
	FName AssociatedQuestID;

	/** Mission/Stage this trigger activates (legacy) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Trigger")
	FName TriggerStageID;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
