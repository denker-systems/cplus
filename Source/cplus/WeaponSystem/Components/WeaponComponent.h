#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSystem/Data/WeaponTypes.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;
class UWeaponDefinition;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged, ABaseWeapon*, NewWeapon, ABaseWeapon*, OldWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MagazineSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReserveAmmoChanged, EAmmoType, AmmoType, int32, CurrentAmount, int32, MaxAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponStateChanged, EWeaponState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFired, ABaseWeapon*, Weapon);

/**
 * Weapon Component
 * 
 * Unified weapon system for both Player and AI characters.
 * Manages: inventory, ammo, reload, weapon slots, firing.
 * 
 * Attach to BasePlayerCharacter or BaseAICharacter.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	// === DELEGATES ===

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnWeaponChanged OnWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnAmmoChanged OnAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnReserveAmmoChanged OnReserveAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnReloadStarted OnReloadStarted;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnReloadFinished OnReloadFinished;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnWeaponStateChanged OnWeaponStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnWeaponFired OnWeaponFired;

protected:
	virtual void BeginPlay() override;

	// === WEAPON INVENTORY ===

	/** Weapons by slot index (0-9 for weapon wheel) */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Inventory")
	TArray<TObjectPtr<ABaseWeapon>> WeaponSlots;

	/** All owned weapons */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Inventory")
	TArray<TObjectPtr<ABaseWeapon>> OwnedWeapons;

	/** Currently equipped weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Inventory")
	TObjectPtr<ABaseWeapon> CurrentWeapon;

	/** Current slot index */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Inventory")
	int32 CurrentSlotIndex = -1;

	// === AMMO INVENTORY ===

	/** Reserve ammo by type */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Ammo")
	TMap<EAmmoType, int32> AmmoInventory;

	/** Maximum reserve ammo by type */
	UPROPERTY(EditAnywhere, Category = "Weapon|Ammo")
	TMap<EAmmoType, int32> MaxAmmoInventory;

	// === STATE ===

	/** Current weapon state */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|State")
	EWeaponState WeaponState = EWeaponState::Idle;

	/** Reload timer handle */
	FTimerHandle ReloadTimerHandle;

	// === CONFIGURATION ===

	/** Starting weapon to spawn on BeginPlay */
	UPROPERTY(EditAnywhere, Category = "Weapon|Config")
	TSubclassOf<ABaseWeapon> StartingWeaponClass;

	/** Starting ammo amounts */
	UPROPERTY(EditAnywhere, Category = "Weapon|Config")
	TMap<EAmmoType, int32> StartingAmmo;

	/** Maximum number of weapon slots */
	UPROPERTY(EditAnywhere, Category = "Weapon|Config", meta = (ClampMin = 1, ClampMax = 10))
	int32 MaxWeaponSlots = 10;

	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "Weapon|Config")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "Weapon|Config")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

public:
	// === WEAPON MANAGEMENT ===

	/** Add a weapon class to inventory */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddWeaponClass(const TSubclassOf<ABaseWeapon>& WeaponClass);

	/** Equip weapon in specific slot */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipSlot(int32 SlotIndex);

	/** Switch to next weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchToNext();

	/** Switch to previous weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchToPrevious();

	/** Legacy support */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchWeapon() { SwitchToNext(); }

	// === FIRING ===

	/** Start firing current weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFiring();

	/** Stop firing current weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFiring();

	// === RELOAD ===

	/** Start reload if possible */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartReload();

	/** Cancel reload in progress */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CancelReload();

	/** Check if can reload current weapon */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanReload() const;

	// === AMMO ===

	/** Get reserve ammo for type */
	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	int32 GetAmmo(EAmmoType AmmoType) const;

	/** Add ammo to reserve */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	void AddAmmo(EAmmoType AmmoType, int32 Amount);

	/** Consume ammo from reserve (returns actual consumed) */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	int32 ConsumeAmmo(EAmmoType AmmoType, int32 Amount);

	/** Check if has ammo for type */
	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	bool HasAmmo(EAmmoType AmmoType) const;

	// === GETTERS ===

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentSlot() const { return CurrentSlotIndex; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	EWeaponState GetWeaponState() const { return WeaponState; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsReloading() const { return WeaponState == EWeaponState::Reloading; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsFiring() const { return WeaponState == EWeaponState::Firing; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	TArray<ABaseWeapon*> GetOwnedWeapons() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* FindWeaponOfType(TSubclassOf<ABaseWeapon> WeaponClass) const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* GetWeaponInSlot(int32 SlotIndex) const;

	/** Deactivate current weapon (called on death) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DeactivateCurrentWeapon();

	/** Update ammo display (legacy support) */
	void UpdateAmmoDisplay(int32 CurrentAmmo, int32 MagazineSize);

protected:
	// === INTERNAL ===

	void SetWeaponState(EWeaponState NewState);
	void FinishReload();
	void InitializeAmmo();
	int32 FindAvailableSlot() const;
};
