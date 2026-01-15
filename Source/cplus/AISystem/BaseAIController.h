#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
class ABaseAICharacter;

/**
 * Base AI Controller
 * 
 * Provides AI perception (sight, hearing, damage) and blackboard integration
 * for behavior trees. Handles target detection and tracking.
 */
UCLASS()
class CPLUS_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// === PERCEPTION ===

	/** AI Perception component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	/** Sight sense config */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	/** Hearing sense config */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	/** Damage sense config */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	/** Sight radius */
	UPROPERTY(EditAnywhere, Category = "AI|Perception", meta = (ClampMin = 100, ClampMax = 10000, Units = "cm"))
	float SightRadius = 3000.0f;

	/** Lose sight radius (should be > SightRadius) */
	UPROPERTY(EditAnywhere, Category = "AI|Perception", meta = (ClampMin = 100, ClampMax = 15000, Units = "cm"))
	float LoseSightRadius = 3500.0f;

	/** Peripheral vision half-angle */
	UPROPERTY(EditAnywhere, Category = "AI|Perception", meta = (ClampMin = 0, ClampMax = 180, Units = "Degrees"))
	float PeripheralVisionAngle = 90.0f;

	/** Hearing range */
	UPROPERTY(EditAnywhere, Category = "AI|Perception", meta = (ClampMin = 100, ClampMax = 10000, Units = "cm"))
	float HearingRange = 2000.0f;

	/** Max age for perception (how long to remember stimuli) */
	UPROPERTY(EditAnywhere, Category = "AI|Perception", meta = (ClampMin = 0, ClampMax = 30, Units = "s"))
	float MaxAge = 5.0f;

	/** Auto-success range for sight (always see within this range) */
	UPROPERTY(EditAnywhere, Category = "AI|Perception", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float AutoSuccessRange = 0.0f;

	/** Called when perception is updated */
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	/** Called when target perception is updated */
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// === BLACKBOARD KEYS ===

	/** Blackboard key for current target actor */
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName TargetActorKey = FName("TargetActor");

	/** Blackboard key for last known target location */
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName LastKnownLocationKey = FName("LastKnownLocation");

	/** Blackboard key for can see target */
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName CanSeeTargetKey = FName("CanSeeTarget");

	/** Blackboard key for is in combat */
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName IsInCombatKey = FName("IsInCombat");

	// === BEHAVIOR ===

	/** Behavior tree to run (if null, will create programmatically) */
	UPROPERTY(EditAnywhere, Category = "AI|Behavior")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	/** If true, create behavior tree programmatically if none is assigned */
	UPROPERTY(EditAnywhere, Category = "AI|Behavior")
	bool bAutoCreateBehaviorTree = true;

	/** Weapon class to equip (used when auto-creating BT) */
	UPROPERTY(EditAnywhere, Category = "AI|Behavior")
	TSubclassOf<class ABaseWeapon> DefaultWeaponClass;

	/** Cached AI character */
	UPROPERTY()
	TObjectPtr<ABaseAICharacter> AICharacter;

	/** Current target actor */
	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget;

public:
	/** Get current target */
	UFUNCTION(BlueprintPure, Category = "AI")
	AActor* GetCurrentTarget() const { return CurrentTarget; }

	/** Set current target (updates blackboard) */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetCurrentTarget(AActor* NewTarget);

	/** Clear current target */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void ClearCurrentTarget();

	/** Check if can see target */
	UFUNCTION(BlueprintPure, Category = "AI")
	bool CanSeeTarget() const;

	/** Get AI character */
	UFUNCTION(BlueprintPure, Category = "AI")
	ABaseAICharacter* GetAICharacter() const { return AICharacter; }
};
