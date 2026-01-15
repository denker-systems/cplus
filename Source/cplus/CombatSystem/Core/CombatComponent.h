#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatTypes.h"
#include "CombatComboData.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class CPLUS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartAttack(EAttackType AttackType);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeTrace(const FMeleeCollisionInfo& CollisionInfo);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetCombo();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatState(ECombatState NewState);

	UFUNCTION(BlueprintPure, Category = "Combat")
	ECombatState GetCombatState() const { return CurrentCombatState; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	int32 GetCurrentComboIndex() const { return CurrentComboIndex; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsAttacking() const { return CurrentCombatState == ECombatState::Attacking; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RegisterHit(AActor* HitActor, const FHitData& HitData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UCombatComboData> DefaultComboData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<TObjectPtr<UCombatComboData>> AvailableCombos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanCombo = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bAutoResetCombo = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECC_Pawn;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackStarted OnAttackStarted;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackEnded OnAttackEnded;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnComboProgressed OnComboProgressed;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnComboReset OnComboReset;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnHitLanded OnHitLanded;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCombatStateChanged OnCombatStateChanged;

protected:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnComboWindowOpened();

	UFUNCTION()
	void OnComboWindowClosed();

	void PlayAttackMontage(const FComboAttack& Attack);
	void ProgressCombo();
	void ProcessHit(const FHitResult& HitResult, float Damage);
	bool HasAlreadyHitActor(AActor* Actor) const;

	UPROPERTY()
	TObjectPtr<UCombatComboData> ActiveComboData = nullptr;

	UPROPERTY()
	ECombatState CurrentCombatState = ECombatState::Idle;

	UPROPERTY()
	int32 CurrentComboIndex = 0;

	UPROPERTY()
	bool bComboWindowOpen = false;

	UPROPERTY()
	bool bAttackQueued = false;

	UPROPERTY()
	EAttackType QueuedAttackType = EAttackType::Light;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActorsThisAttack;

	FTimerHandle ComboResetTimerHandle;
	FTimerHandle ComboWindowTimerHandle;
};
