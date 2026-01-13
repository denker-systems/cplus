#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseAICharacter.generated.h"

UENUM(BlueprintType)
enum class EAITeam : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Friendly UMETA(DisplayName = "Friendly"),
	Hostile UMETA(DisplayName = "Hostile")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIDeath, AActor*, Killer);

/**
 * Base AI Character
 * 
 * Base class for all AI-controlled characters
 * Provides: Health, Team, StateTree, Death handling
 */
UCLASS(Abstract)
class CPLUS_API ABaseAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

	/** Death event */
	UPROPERTY(BlueprintAssignable, Category = "AI")
	FOnAIDeath OnAIDeath;

protected:
	virtual void BeginPlay() override;

	/** Current health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Health")
	float CurrentHP = 100.0f;

	/** Maximum health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Health")
	float MaxHP = 100.0f;

	/** AI team */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Team")
	EAITeam Team = EAITeam::Neutral;

	/** Is this AI dead? */
	UPROPERTY(BlueprintReadOnly, Category = "AI|Health")
	bool bIsDead = false;

	/** Quest target component (optional) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UQuestTargetComponent* QuestTarget;

public:
	/** Take damage */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, 
		AController* EventInstigator, AActor* DamageCauser) override;

	/** Check if dead */
	UFUNCTION(BlueprintPure, Category = "AI")
	bool IsDead() const { return bIsDead; }

	/** Get team */
	UFUNCTION(BlueprintPure, Category = "AI")
	EAITeam GetTeam() const { return Team; }

protected:
	/** Called when HP reaches zero */
	virtual void Die(AActor* Killer);
};
