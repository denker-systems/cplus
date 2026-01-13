#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AShooterWeapon;
class IShooterWeaponHolder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged, AShooterWeapon*, NewWeapon, AShooterWeapon*, OldWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MagazineSize);

/**
 * Weapon Component
 * 
 * Manages weapon inventory and switching for shooter characters
 * Handles weapon spawning, activation, and deactivation
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	/** Weapon changed event */
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponChanged OnWeaponChanged;

	/** Ammo changed event */
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChanged OnAmmoChanged;

protected:
	virtual void BeginPlay() override;

	/** List of weapons owned by this character */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TArray<TObjectPtr<AShooterWeapon>> OwnedWeapons;

	/** Currently equipped weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AShooterWeapon> CurrentWeapon;

	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

public:
	/** Add a weapon class to inventory */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass);

	/** Switch to next weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchWeapon();

	/** Start firing current weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFiring();

	/** Stop firing current weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFiring();

	/** Get current weapon */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AShooterWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	/** Get all owned weapons */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	TArray<AShooterWeapon*> GetOwnedWeapons() const;

	/** Check if we own a weapon of this type */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AShooterWeapon* FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const;

	/** Deactivate current weapon (called on death) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DeactivateCurrentWeapon();

	/** Update ammo display */
	void UpdateAmmoDisplay(int32 CurrentAmmo, int32 MagazineSize);
};
