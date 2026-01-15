#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasWeapon.generated.h"

/**
 * BTDecorator: Has Weapon
 * 
 * Checks if AI has a weapon equipped.
 */
UCLASS()
class CPLUS_API UBTDecorator_HasWeapon : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_HasWeapon();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	/** If true, check for NO weapon (inverted) */
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bInvertCondition = false;
};
