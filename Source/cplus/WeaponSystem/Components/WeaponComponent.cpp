#include "WeaponSystem/Components/WeaponComponent.h"
#include "WeaponSystem/Actors/BaseWeapon.h"
#include "WeaponSystem/IWeaponHolder.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/ItemDefinition.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize weapon slots
	WeaponSlots.SetNum(10);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache inventory component reference
	CacheInventoryComponent();

	// Initialize ammo inventory (fallback for AI without inventory)
	InitializeAmmo();

	// Spawn starting weapon if configured
	if (StartingWeaponClass)
	{
		AddWeaponClass(StartingWeaponClass);
	}
}

void UWeaponComponent::CacheInventoryComponent()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		InventoryComponent = Owner->FindComponentByClass<UInventoryComponent>();
	}
}

void UWeaponComponent::InitializeAmmo()
{
	// Set up starting ammo
	for (const auto& Pair : StartingAmmo)
	{
		AmmoInventory.Add(Pair.Key, Pair.Value);
	}

	// Initialize max ammo defaults if not set
	if (MaxAmmoInventory.Num() == 0)
	{
		MaxAmmoInventory.Add(EAmmoType::Pistol, 200);
		MaxAmmoInventory.Add(EAmmoType::Rifle, 300);
		MaxAmmoInventory.Add(EAmmoType::Shotgun, 50);
		MaxAmmoInventory.Add(EAmmoType::Sniper, 30);
		MaxAmmoInventory.Add(EAmmoType::Explosive, 10);
		MaxAmmoInventory.Add(EAmmoType::Energy, 100);
		MaxAmmoInventory.Add(EAmmoType::Special, 20);
	}
}

void UWeaponComponent::AddWeaponClass(const TSubclassOf<ABaseWeapon>& WeaponClass)
{
	if (!WeaponClass)
	{
		return;
	}

	// Do we already own this weapon?
	ABaseWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);
	if (OwnedWeapon)
	{
		return;
	}

	// Get owner actor
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// Spawn the new weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Cast<APawn>(Owner);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	ABaseWeapon* AddedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, Owner->GetActorTransform(), SpawnParams);

	if (AddedWeapon)
	{
		// Add the weapon to the owned list
		OwnedWeapons.Add(AddedWeapon);

		// Find available slot
		int32 SlotIndex = FindAvailableSlot();
		if (SlotIndex >= 0 && SlotIndex < WeaponSlots.Num())
		{
			WeaponSlots[SlotIndex] = AddedWeapon;
		}

		// If we have an existing weapon, deactivate it
		if (CurrentWeapon)
		{
			CurrentWeapon->DeactivateWeapon();
		}

		// Switch to the new weapon
		ABaseWeapon* OldWeapon = CurrentWeapon;
		CurrentWeapon = AddedWeapon;
		CurrentSlotIndex = SlotIndex;
		CurrentWeapon->ActivateWeapon();

		SetWeaponState(EWeaponState::Idle);

		// Broadcast weapon changed
		OnWeaponChanged.Broadcast(CurrentWeapon, OldWeapon);
	}
}

void UWeaponComponent::EquipSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= WeaponSlots.Num())
	{
		return;
	}

	ABaseWeapon* WeaponInSlot = WeaponSlots[SlotIndex];
	if (!WeaponInSlot || WeaponInSlot == CurrentWeapon)
	{
		return;
	}

	// Can't switch while reloading
	if (WeaponState == EWeaponState::Reloading)
	{
		CancelReload();
	}

	// Deactivate old weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->DeactivateWeapon();
	}

	// Switch to new weapon
	ABaseWeapon* OldWeapon = CurrentWeapon;
	CurrentWeapon = WeaponInSlot;
	CurrentSlotIndex = SlotIndex;
	CurrentWeapon->ActivateWeapon();

	SetWeaponState(EWeaponState::Idle);

	// Broadcast weapon changed
	OnWeaponChanged.Broadcast(CurrentWeapon, OldWeapon);
}

void UWeaponComponent::SwitchToNext()
{
	if (OwnedWeapons.Num() <= 1 || !CurrentWeapon)
	{
		return;
	}

	// Find next occupied slot
	int32 StartSlot = CurrentSlotIndex;
	int32 NextSlot = (CurrentSlotIndex + 1) % WeaponSlots.Num();

	while (NextSlot != StartSlot)
	{
		if (WeaponSlots[NextSlot])
		{
			EquipSlot(NextSlot);
			return;
		}
		NextSlot = (NextSlot + 1) % WeaponSlots.Num();
	}
}

void UWeaponComponent::SwitchToPrevious()
{
	if (OwnedWeapons.Num() <= 1 || !CurrentWeapon)
	{
		return;
	}

	// Find previous occupied slot
	int32 StartSlot = CurrentSlotIndex;
	int32 PrevSlot = (CurrentSlotIndex - 1 + WeaponSlots.Num()) % WeaponSlots.Num();

	while (PrevSlot != StartSlot)
	{
		if (WeaponSlots[PrevSlot])
		{
			EquipSlot(PrevSlot);
			return;
		}
		PrevSlot = (PrevSlot - 1 + WeaponSlots.Num()) % WeaponSlots.Num();
	}
}

void UWeaponComponent::RemoveCurrentWeapon()
{
	if (!CurrentWeapon)
	{
		return;
	}

	// Deactivate current weapon
	ABaseWeapon* OldWeapon = CurrentWeapon;
	CurrentWeapon->DeactivateWeapon();

	// Clear current weapon
	CurrentWeapon = nullptr;
	CurrentSlotIndex = -1;

	SetWeaponState(EWeaponState::Disabled);

	// Broadcast weapon changed (nullptr = no weapon)
	OnWeaponChanged.Broadcast(nullptr, OldWeapon);
}

void UWeaponComponent::StartFiring()
{
	if (!CurrentWeapon || WeaponState == EWeaponState::Reloading || WeaponState == EWeaponState::Disabled)
	{
		return;
	}

	SetWeaponState(EWeaponState::Firing);
	CurrentWeapon->StartFiring();
	OnWeaponFired.Broadcast(CurrentWeapon);
}

void UWeaponComponent::StopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}

	if (WeaponState == EWeaponState::Firing)
	{
		SetWeaponState(EWeaponState::Idle);
	}
}

void UWeaponComponent::StartReload()
{
	UE_LOG(LogTemp, Display, TEXT(">>> WEAPON: StartReload called"));
	
	if (!CanReload())
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON: CanReload returned false"));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> WEAPON: CanReload passed, starting reload"));

	// Stop firing first
	StopFiring();

	SetWeaponState(EWeaponState::Reloading);
	OnReloadStarted.Broadcast();

	// Get reload time from weapon (default 2 seconds)
	float ReloadTime = 2.0f;
	// TODO: Get from WeaponDefinition when integrated

	// Set timer for reload completion
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&UWeaponComponent::FinishReload,
		ReloadTime,
		false
	);
}

void UWeaponComponent::CancelReload()
{
	if (WeaponState == EWeaponState::Reloading)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		SetWeaponState(EWeaponState::Idle);
	}
}

bool UWeaponComponent::CanReload() const
{
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON: CanReload - CurrentWeapon is NULL"));
		return false;
	}

	if (WeaponState == EWeaponState::Reloading || WeaponState == EWeaponState::Disabled)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON: CanReload - WeaponState is Reloading or Disabled"));
		return false;
	}

	// Check if magazine is not full
	int32 CurrentAmmo = CurrentWeapon->GetBulletCount();
	int32 MagSize = CurrentWeapon->GetMagazineSize();
	UE_LOG(LogTemp, Display, TEXT(">>> WEAPON: CanReload - CurrentAmmo=%d, MagSize=%d"), CurrentAmmo, MagSize);

	if (CurrentAmmo >= MagSize)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON: CanReload - Magazine already full"));
		return false;
	}

	// Check reserve ammo (skip if AmmoType::None = infinite)
	EAmmoType WeaponAmmoType = CurrentWeapon->GetAmmoType();
	UE_LOG(LogTemp, Display, TEXT(">>> WEAPON: CanReload - AmmoType=%d"), (int32)WeaponAmmoType);
	
	if (WeaponAmmoType != EAmmoType::None && !HasReserveAmmo(WeaponAmmoType))
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> WEAPON: CanReload - No reserve ammo"));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> WEAPON: CanReload - All checks passed, can reload"));
	return true;
}

void UWeaponComponent::FinishReload()
{
	if (!CurrentWeapon)
	{
		SetWeaponState(EWeaponState::Idle);
		return;
	}

	// Fill magazine from reserve ammo
	int32 MagSize = CurrentWeapon->GetMagazineSize();
	int32 CurrentAmmo = CurrentWeapon->GetBulletCount();
	int32 NeededAmmo = MagSize - CurrentAmmo;

	// Get ammo type from weapon
	EAmmoType WeaponAmmoType = CurrentWeapon->GetAmmoType();

	// Consume from reserve (or infinite if AmmoType::None)
	int32 AmmoToAdd = NeededAmmo;
	if (WeaponAmmoType != EAmmoType::None)
	{
		AmmoToAdd = ConsumeReserveAmmo(WeaponAmmoType, NeededAmmo);
	}

	// Add bullets to weapon magazine
	if (AmmoToAdd > 0)
	{
		CurrentWeapon->AddBullets(AmmoToAdd);
	}

	SetWeaponState(EWeaponState::Idle);
	OnReloadFinished.Broadcast();

	// Update ammo display
	UpdateAmmoDisplay(CurrentWeapon->GetBulletCount(), MagSize);
}

int32 UWeaponComponent::GetAmmo(EAmmoType AmmoType) const
{
	const int32* Amount = AmmoInventory.Find(AmmoType);
	return Amount ? *Amount : 0;
}

void UWeaponComponent::AddAmmo(EAmmoType AmmoType, int32 Amount)
{
	if (AmmoType == EAmmoType::None || Amount <= 0)
	{
		return;
	}

	int32 CurrentAmount = GetAmmo(AmmoType);
	int32 MaxAmount = MaxAmmoInventory.Contains(AmmoType) ? MaxAmmoInventory[AmmoType] : 999;
	int32 NewAmount = FMath::Min(CurrentAmount + Amount, MaxAmount);

	AmmoInventory.Add(AmmoType, NewAmount);
	OnReserveAmmoChanged.Broadcast(AmmoType, NewAmount, MaxAmount);
}

int32 UWeaponComponent::ConsumeAmmo(EAmmoType AmmoType, int32 Amount)
{
	if (AmmoType == EAmmoType::None)
	{
		return Amount; // Infinite ammo for None type
	}

	int32 CurrentAmount = GetAmmo(AmmoType);
	int32 ConsumedAmount = FMath::Min(CurrentAmount, Amount);

	AmmoInventory.Add(AmmoType, CurrentAmount - ConsumedAmount);

	int32 MaxAmount = MaxAmmoInventory.Contains(AmmoType) ? MaxAmmoInventory[AmmoType] : 999;
	OnReserveAmmoChanged.Broadcast(AmmoType, CurrentAmount - ConsumedAmount, MaxAmount);

	return ConsumedAmount;
}

bool UWeaponComponent::HasAmmo(EAmmoType AmmoType) const
{
	if (AmmoType == EAmmoType::None)
	{
		return true; // Infinite ammo for None type
	}
	return GetAmmo(AmmoType) > 0;
}

TArray<ABaseWeapon*> UWeaponComponent::GetOwnedWeapons() const
{
	TArray<ABaseWeapon*> Result;
	for (ABaseWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon)
		{
			Result.Add(Weapon);
		}
	}
	return Result;
}

ABaseWeapon* UWeaponComponent::FindWeaponOfType(TSubclassOf<ABaseWeapon> WeaponClass) const
{
	for (ABaseWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon && Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}
	return nullptr;
}

ABaseWeapon* UWeaponComponent::GetWeaponInSlot(int32 SlotIndex) const
{
	if (SlotIndex >= 0 && SlotIndex < WeaponSlots.Num())
	{
		return WeaponSlots[SlotIndex];
	}
	return nullptr;
}

void UWeaponComponent::DeactivateCurrentWeapon()
{
	CancelReload();
	StopFiring();

	if (CurrentWeapon)
	{
		CurrentWeapon->DeactivateWeapon();
	}

	SetWeaponState(EWeaponState::Disabled);
}

void UWeaponComponent::UpdateAmmoDisplay(int32 CurrentAmmo, int32 MagazineSize)
{
	OnAmmoChanged.Broadcast(CurrentAmmo, MagazineSize);
}

void UWeaponComponent::SetWeaponState(EWeaponState NewState)
{
	if (WeaponState != NewState)
	{
		WeaponState = NewState;
		OnWeaponStateChanged.Broadcast(NewState);
	}
}

int32 UWeaponComponent::FindAvailableSlot() const
{
	for (int32 i = 0; i < WeaponSlots.Num(); ++i)
	{
		if (!WeaponSlots[i])
		{
			return i;
		}
	}
	return -1;
}

// === INVENTORY AMMO BRIDGE ===

int32 UWeaponComponent::GetReserveAmmo(EAmmoType AmmoType) const
{
	if (AmmoType == EAmmoType::None)
	{
		return 999; // Infinite
	}

	// Try inventory first if enabled
	if (bUseInventoryAmmo && IsValid(InventoryComponent))
	{
		const TObjectPtr<UItemDefinition>* AmmoItem = AmmoTypeToItem.Find(AmmoType);
		if (AmmoItem && IsValid(*AmmoItem))
		{
			return InventoryComponent->GetItemQuantity(*AmmoItem);
		}
	}

	// Fallback to internal storage
	return GetAmmo(AmmoType);
}

void UWeaponComponent::AddReserveAmmo(EAmmoType AmmoType, int32 Amount)
{
	if (AmmoType == EAmmoType::None || Amount <= 0)
	{
		return;
	}

	// Try inventory first if enabled
	if (bUseInventoryAmmo && IsValid(InventoryComponent))
	{
		const TObjectPtr<UItemDefinition>* AmmoItem = AmmoTypeToItem.Find(AmmoType);
		if (AmmoItem && IsValid(*AmmoItem))
		{
			InventoryComponent->AddItem(*AmmoItem, Amount);
			return;
		}
	}

	// Fallback to internal storage
	AddAmmo(AmmoType, Amount);
}

int32 UWeaponComponent::ConsumeReserveAmmo(EAmmoType AmmoType, int32 Amount)
{
	if (AmmoType == EAmmoType::None)
	{
		return Amount; // Infinite
	}

	// Try inventory first if enabled
	if (bUseInventoryAmmo && IsValid(InventoryComponent))
	{
		const TObjectPtr<UItemDefinition>* AmmoItem = AmmoTypeToItem.Find(AmmoType);
		if (AmmoItem && IsValid(*AmmoItem))
		{
			int32 Available = InventoryComponent->GetItemQuantity(*AmmoItem);
			int32 ToConsume = FMath::Min(Available, Amount);
			InventoryComponent->RemoveItem(*AmmoItem, ToConsume);
			return ToConsume;
		}
	}

	// Fallback to internal storage
	return ConsumeAmmo(AmmoType, Amount);
}

bool UWeaponComponent::HasReserveAmmo(EAmmoType AmmoType) const
{
	if (AmmoType == EAmmoType::None)
	{
		return true; // Infinite
	}

	return GetReserveAmmo(AmmoType) > 0;
}
