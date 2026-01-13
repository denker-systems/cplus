#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, HealthPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, Killer);

/**
 * Health Component
 * 
 * Manages health, damage, and death for any actor
 * Can be used by both players and AI
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	/** Health changed event */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	/** Death event */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	/** Maximum health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

	/** Current health */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth = 0.0f;

	/** Is this actor dead? */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	/** Can this actor take damage? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bCanTakeDamage = true;

public:
	/** Apply damage to this actor */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float ApplyDamage(float Damage, AActor* DamageCauser);

	/** Heal this actor */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Amount);

	/** Set health to max */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ResetHealth();

	/** Check if dead */
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

	/** Get current health */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	/** Get max health */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** Get health percent (0.0 - 1.0) */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

protected:
	/** Called when health reaches zero */
	virtual void Die(AActor* Killer);
};
