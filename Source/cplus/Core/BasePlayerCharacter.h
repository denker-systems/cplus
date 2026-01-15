#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "WeaponSystem/IWeaponHolder.h"
#include "BasePlayerCharacter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;
class UInputAction;
class UPawnNoiseEmitterComponent;
class UHealthComponent;
class UWeaponComponent;
class UPlayerProgressionComponent;
class UUIManager;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprintStateChanged, bool, bSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprintMeterUpdated, float, Percentage);

/**
 * Base Player Character
 * 
 * Consolidated base class for all player-controlled characters
 * Provides: FPS camera/mesh, Quest system, Inventory, Interaction,
 *           Health system, Weapon system, Sprint/Stamina system
 * 
 * Features can be enabled/disabled via EditAnywhere properties
 */
UCLASS(Abstract)
class CPLUS_API ABasePlayerCharacter : public ABaseCharacter, public IWeaponHolder
{
	GENERATED_BODY()

protected:

	/** Spring arm for third-person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Third-person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Spotlight for horror mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLight;

	/** Pawn noise emitter for AI detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;

public:
	ABasePlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	// === CORE COMPONENTS ===

	/** Quest tracker component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UQuestTrackerComponent* QuestTracker;

	/** Inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* Inventory;

	/** Interaction component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* InteractionComponent;

	/** Health component - ADD MANUALLY IN BLUEPRINT */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	/** Weapon component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponComponent* WeaponComponent;

	/** Player progression component (XP, Level, Gold) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerProgressionComponent* ProgressionComponent;

	/** UI Manager (handles ALL UI: pause menu, quest UI, inventory, etc) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUIManager* UIManager;

	// === INPUT ACTIONS ===

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	/** Switch weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SwitchWeaponAction;

	/** Sprint input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	/** Crouch input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	/** Interact input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	/** Open quest journal input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenJournalAction;

	/** Pause menu input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	/** Toggle inventory input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InventoryAction;

	/** Reload weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;

	// === ANIMATION ===

	/** Unarmed animation blueprint class (when no weapon equipped) */
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> UnarmedAnimInstanceClass;

	// === INTERACTION ===

	/** Max distance for interaction raycast */
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float InteractionDistance = 300.0f;

	/** Currently focused interactable actor */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> CurrentInteractable;

	/** Enable debug visualization for interaction raycast */
	UPROPERTY(EditAnywhere, Category = "Interaction|Debug")
	bool bShowInteractionDebug = false;


	// === SHOOTER FEATURES ===

	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

	/** Max distance to use for aim traces */
	UPROPERTY(EditAnywhere, Category = "Aim", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float MaxAimDistance = 10000.0f;

	/** Team ID for this character */
	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamByte = 0;

	/** Actor tag to grant this character when it dies */
	UPROPERTY(EditAnywhere, Category = "Team")
	FName DeathTag = FName("Dead");

	/** Respawn time after death */
	UPROPERTY(EditAnywhere, Category = "Destruction", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RespawnTime = 5.0f;

	FTimerHandle RespawnTimer;

	// === HORROR FEATURES (SPRINT/STAMINA) ===

	/** If true, we're sprinting */
	bool bSprinting = false;

	/** If true, we're recovering stamina */
	bool bRecovering = false;

	/** Default walk speed when not sprinting or recovering */
	UPROPERTY(EditAnywhere, Category = "Walk")
	float WalkSpeed = 250.0f;

	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 3.0f;

	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float SprintSpeed = 600.0f;

	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category = "Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category = "Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	FTimerHandle SprintTimer;

public:
	/** Sprint state changed delegate */
	UPROPERTY(BlueprintAssignable, Category = "Sprint")
	FOnSprintStateChanged OnSprintStateChanged;

	/** Sprint meter updated delegate */
	UPROPERTY(BlueprintAssignable, Category = "Sprint")
	FOnSprintMeterUpdated OnSprintMeterUpdated;

protected:
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// === INPUT HANDLERS ===

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** Handles start firing input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartFiring();

	/** Handles stop firing input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStopFiring();

	/** Handles switch weapon input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoSwitchWeapon();

	/** Handles reload input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoReload();

	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoEndSprint();

	/** Starts crouching behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartCrouch();

	/** Stops crouching behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoEndCrouch();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();

	// === INTERACTION ===

	/** Check for interactable actors in front of player */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CheckForInteractable();

	/** Perform interaction with current interactable */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DoInteract();

	/** Open/toggle quest journal */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void DoOpenJournal();

	/** Toggle pause menu */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DoPauseMenu();

	/** Toggle inventory */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DoToggleInventory();

	/** Blueprint event called when quest is offered to player */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void OnQuestOfferedToPlayer(UQuestDefinition* Quest, AActor* QuestGiver);

	/** Show quest dialog widget */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ShowQuestDialog(UQuestDefinition* Quest, AActor* QuestGiver);

	/** Show quest completion dialog widget */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ShowQuestCompletionDialog(UQuestDefinition* Quest, const struct FQuestReward& Rewards, AActor* QuestGiver);

private:
	/** Internal handler for quest offered delegate */
	UFUNCTION()
	void HandleQuestOffered(UQuestDefinition* Quest, AActor* QuestGiver);

	/** Internal handler for quest turned in delegate */
	UFUNCTION()
	void HandleQuestTurnedIn(FName QuestID, UQuestDefinition* Quest, AActor* QuestGiver);

protected:
	// === DEATH/RESPAWN ===

	/** Called when this character's HP is depleted */
	UFUNCTION()
	void OnHealthDepleted(AActor* Killer);

	/** Called to allow Blueprint code to react to this character's death */
	UFUNCTION(BlueprintImplementableEvent, Category = "Player", meta = (DisplayName = "On Death"))
	void BP_OnDeath();

	/** Called from the respawn timer to destroy this character and force the PC to respawn */
	void OnRespawn();

public:
	// === IWeaponHolder INTERFACE ===

	virtual void AttachWeaponMeshes(class ABaseWeapon* Weapon) override;
	virtual void PlayFiringMontage(UAnimMontage* Montage) override;
	virtual void AddWeaponRecoil(float Recoil) override;
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;
	virtual FVector GetWeaponTargetLocation() override;
	virtual void AddWeaponClass(const TSubclassOf<class ABaseWeapon>& WeaponClass) override;
	virtual void OnWeaponActivated(class ABaseWeapon* Weapon) override;
	virtual void OnWeaponDeactivated(class ABaseWeapon* Weapon) override;
	virtual void OnSemiWeaponRefire() override;

	// === GETTERS ===

	/** Get quest tracker */
	UFUNCTION(BlueprintPure, Category = "Player")
	UQuestTrackerComponent* GetQuestTracker() const { return QuestTracker; }

	/** Get inventory */
	UFUNCTION(BlueprintPure, Category = "Player")
	UInventoryComponent* GetInventory() const { return Inventory; }

	/** Get interaction component */
	UFUNCTION(BlueprintPure, Category = "Player")
	UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	/** Get health component */
	UFUNCTION(BlueprintPure, Category = "Player")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	/** Get weapon component */
	UFUNCTION(BlueprintPure, Category = "Player")
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	/** Get progression component */
	UFUNCTION(BlueprintPure, Category = "Player")
	UPlayerProgressionComponent* GetProgressionComponent() const { return ProgressionComponent; }

	/** Get UI manager */
	UFUNCTION(BlueprintPure, Category = "Player")
	UUIManager* GetUIManager() const { return UIManager; }

	/** Returns the camera boom */
	UFUNCTION(BlueprintPure, Category = "Player")
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns follow camera component */
	UFUNCTION(BlueprintPure, Category = "Player")
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns true if the character is dead */
	UFUNCTION(BlueprintPure, Category = "Player")
	bool IsDead() const;

	/** Handle incoming damage */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
