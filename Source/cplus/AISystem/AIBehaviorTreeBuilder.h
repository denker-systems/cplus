#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIBehaviorTreeBuilder.generated.h"

class UBehaviorTree;
class UBlackboardData;
class ABaseAIController;

/**
 * AI Behavior Tree Builder
 * 
 * Utility class for creating Behavior Trees and Blackboards programmatically.
 * Used to avoid manual Editor setup for AI.
 */
UCLASS()
class CPLUS_API UAIBehaviorTreeBuilder : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Create a basic combat Behavior Tree with Blackboard
	 * 
	 * Tree structure:
	 * Root
	 *   Selector
	 *     Sequence [Has Target]
	 *       - Decorator: Has Target
	 *       - Service: Update Target
	 *       - Sequence [Combat]
	 *         - Task: Equip Weapon
	 *         - Task: Move To Target
	 *         - Task: Start Combat
	 *     Sequence [Idle]
	 *       - Task: Wait
	 */
	static UBehaviorTree* CreateCombatBehaviorTree(UObject* Outer, TSubclassOf<class ABaseWeapon> WeaponClass);

	/**
	 * Create Blackboard for AI combat
	 * 
	 * Keys:
	 * - TargetActor (Object)
	 * - LastKnownLocation (Vector)
	 * - CanSeeTarget (Bool)
	 * - IsInCombat (Bool)
	 */
	static UBlackboardData* CreateCombatBlackboard(UObject* Outer);
};
