#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopCombat.generated.h"

/**
 * BTTask: Stop Combat
 * 
 * Stops shooting.
 */
UCLASS()
class CPLUS_API UBTTask_StopCombat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StopCombat();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
