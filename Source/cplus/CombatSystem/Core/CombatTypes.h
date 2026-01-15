#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatTypes.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Attacking UMETA(DisplayName = "Attacking"),
	Blocking UMETA(DisplayName = "Blocking"),
	Dodging UMETA(DisplayName = "Dodging"),
	Stunned UMETA(DisplayName = "Stunned"),
	Disabled UMETA(DisplayName = "Disabled")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Light UMETA(DisplayName = "Light Attack"),
	Heavy UMETA(DisplayName = "Heavy Attack"),
	Special UMETA(DisplayName = "Special Attack"),
	Finisher UMETA(DisplayName = "Finisher")
};

UENUM(BlueprintType)
enum class EHitReaction : uint8
{
	None UMETA(DisplayName = "None"),
	Light UMETA(DisplayName = "Light"),
	Medium UMETA(DisplayName = "Medium"),
	Heavy UMETA(DisplayName = "Heavy"),
	Knockback UMETA(DisplayName = "Knockback"),
	Knockdown UMETA(DisplayName = "Knockdown")
};

UENUM(BlueprintType)
enum class EMeleeCollisionType : uint8
{
	LeftHand UMETA(DisplayName = "Left Hand"),
	RightHand UMETA(DisplayName = "Right Hand"),
	LeftFoot UMETA(DisplayName = "Left Foot"),
	RightFoot UMETA(DisplayName = "Right Foot"),
	Head UMETA(DisplayName = "Head"),
	Weapon UMETA(DisplayName = "Weapon")
};

USTRUCT(BlueprintType)
struct FComboAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAttackType AttackType = EAttackType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ComboWindowStart = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ComboWindowEnd = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName NextComboSection = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanBeInterrupted = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float StaminaCost = 10.0f;
};

USTRUCT(BlueprintType)
struct FMeleeCollisionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EMeleeCollisionType CollisionType = EMeleeCollisionType::RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TraceRadius = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bDrawDebug = false;
};

USTRUCT(BlueprintType)
struct FHitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> HitActor = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FVector HitLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FVector HitNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float Damage = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	EHitReaction HitReaction = EHitReaction::Light;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackStarted, EAttackType, AttackType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboProgressed, int32, ComboIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitLanded, AActor*, HitActor, const FHitData&, HitData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStateChanged, ECombatState, NewState);
