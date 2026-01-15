#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartCombat.generated.h"

/**
 * BTTask: Start Combat
 * 
 * Starts shooting at the target actor from blackboard.
 */
UCLASS()
class CPLUS_API UBTTask_StartCombat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StartCombat();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	/** Blackboard key for target actor */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
