#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EquipWeapon.generated.h"

/**
 * BTTask: Equip Weapon
 * 
 * Gives the AI a weapon class and equips it.
 * Used at the start of combat behavior.
 */
UCLASS()
class CPLUS_API UBTTask_EquipWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EquipWeapon();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	/** Weapon class to equip */
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<class ABaseWeapon> WeaponClass;

	/** If true, only equip if AI doesn't already have a weapon */
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bOnlyIfUnarmed = true;
};
