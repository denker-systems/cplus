#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateTarget.generated.h"

/**
 * BTService: Update Target
 * 
 * Continuously updates target information in blackboard.
 * Checks line of sight and updates last known location.
 */
UCLASS()
class CPLUS_API UBTService_UpdateTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateTarget();

public:
	/** Blackboard key for target actor */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** Blackboard key for can see target */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CanSeeTargetKey;

	/** Blackboard key for last known location */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LastKnownLocationKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	/** If true, clear target if lost sight for too long */
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bClearTargetIfLost = true;

	/** Time to wait before clearing lost target */
	UPROPERTY(EditAnywhere, Category = "AI", meta = (EditCondition = "bClearTargetIfLost", ClampMin = 0, ClampMax = 30, Units = "s"))
	float LostTargetTimeout = 10.0f;

private:
	/** Time since target was last seen */
	float TimeSinceLastSeen = 0.0f;
};
