#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "QuestInteractable.h"
#include "QuestKillable.h"
#include "ShooterWeaponHolder.h"
#include "BaseAICharacter.generated.h"

class UHealthComponent;
class UWeaponComponent;
class AShooterWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIPawnDeath);

UENUM(BlueprintType)
enum class EAITeam : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Friendly UMETA(DisplayName = "Friendly"),
	Hostile UMETA(DisplayName = "Hostile")
};

/**
 * Base AI Character
 * 
 * Consolidated base class for all AI-controlled characters
 * Provides: Health, Team, Weapon system, Quest integration,
 *           Shooter features (aim, shooting), Death handling
 * 
 * Features can be enabled/disabled via EditAnywhere properties
 */
UCLASS(Abstract)
class CPLUS_API ABaseAICharacter : public ABaseCharacter, public IQuestInteractable, public IQuestKillable, public IShooterWeaponHolder
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

	/** Death event */
	UPROPERTY(BlueprintAssignable, Category = "AI")
	FOnAIPawnDeath OnAIPawnDeath;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	// === COMPONENTS ===

	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	/** Weapon component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponComponent* WeaponComponent;

	/** Quest target component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UQuestTargetComponent* QuestTarget;

	/** Quest giver component (for friendly NPCs) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UQuestGiverComponent* QuestGiver;

	/** Interaction sphere for quest/interaction detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

	/** Interaction radius for this NPC */
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (ClampMin = 50, ClampMax = 500, Units = "cm"))
	float InteractionRadius = 200.0f;

	/** Enable debug visualization for interaction sphere */
	UPROPERTY(EditAnywhere, Category = "Interaction|Debug")
	bool bShowInteractionSphere = false;

	// === AI SETTINGS ===

	/** AI team */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Team")
	EAITeam Team = EAITeam::Neutral;

	/** Team byte for scoring */
	UPROPERTY(EditAnywhere, Category = "AI|Team")
	uint8 TeamByte = 1;

	/** Actor tag to grant this character when it dies */
	UPROPERTY(EditAnywhere, Category = "AI|Team")
	FName DeathTag = FName("Dead");

	/** NPC identifier (for friendly NPCs) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Identity")
	FName NPCID;

	// === SHOOTER FEATURES ===

	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "AI|Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "AI|Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

	/** Max range for aiming calculations */
	UPROPERTY(EditAnywhere, Category = "AI|Aim")
	float AimRange = 10000.0f;

	/** Cone variance to apply while aiming */
	UPROPERTY(EditAnywhere, Category = "AI|Aim")
	float AimVarianceHalfAngle = 10.0f;

	/** Minimum vertical offset from the target center to apply when aiming */
	UPROPERTY(EditAnywhere, Category = "AI|Aim")
	float MinAimOffsetZ = -35.0f;

	/** Maximum vertical offset from the target center to apply when aiming */
	UPROPERTY(EditAnywhere, Category = "AI|Aim")
	float MaxAimOffsetZ = -60.0f;

	/** Actor currently being targeted */
	TObjectPtr<AActor> CurrentAimTarget;

	/** If true, this character is currently shooting its weapon */
	bool bIsShooting = false;

	// === DEATH/RAGDOLL ===

	/** Name of the collision profile to use during ragdoll death */
	UPROPERTY(EditAnywhere, Category = "AI|Death")
	FName RagdollCollisionProfile = FName("Ragdoll");

	/** Time to wait after death before destroying this actor */
	UPROPERTY(EditAnywhere, Category = "AI|Death")
	float DeferredDestructionTime = 5.0f;

	FTimerHandle DeathTimer;

public:
	// === IQuestInteractable INTERFACE ===

	virtual void Interact_Implementation(AActor* Interactor) override;

	// === IQuestKillable INTERFACE ===

	virtual void OnKilledForQuest_Implementation(AActor* Killer) override;

	// === IShooterWeaponHolder INTERFACE ===

	virtual void AttachWeaponMeshes(AShooterWeapon* Weapon) override;
	virtual void PlayFiringMontage(UAnimMontage* Montage) override;
	virtual void AddWeaponRecoil(float Recoil) override;
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;
	virtual FVector GetWeaponTargetLocation() override;
	virtual void AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass) override;
	virtual void OnWeaponActivated(AShooterWeapon* Weapon) override;
	virtual void OnWeaponDeactivated(AShooterWeapon* Weapon) override;
	virtual void OnSemiWeaponRefire() override;

	// === SHOOTING API ===

	/** Signals this character to start shooting at the passed actor */
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void StartShooting(AActor* ActorToShoot);

	/** Signals this character to stop shooting */
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void StopShooting();

	// === GETTERS ===

	/** Check if dead */
	UFUNCTION(BlueprintPure, Category = "AI")
	bool IsDead() const;

	/** Get team */
	UFUNCTION(BlueprintPure, Category = "AI")
	EAITeam GetTeam() const { return Team; }

	/** Get NPC ID */
	UFUNCTION(BlueprintPure, Category = "AI")
	FName GetNPCID() const { return NPCID; }

	/** Get health component */
	UFUNCTION(BlueprintPure, Category = "AI")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	/** Get weapon component */
	UFUNCTION(BlueprintPure, Category = "AI")
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	/** Handle incoming damage */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, 
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	/** Called when HP reaches zero */
	UFUNCTION()
	virtual void OnHealthDepleted(AActor* Killer);

	/** Called after death to destroy the actor */
	void DeferredDestruction();
};
