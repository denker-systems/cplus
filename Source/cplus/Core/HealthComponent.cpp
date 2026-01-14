#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health to max
	CurrentHealth = MaxHealth;
}

float UHealthComponent::ApplyDamage(float Damage, AActor* DamageCauser)
{
	if (bIsDead || !bCanTakeDamage)
	{
		return 0.0f;
	}

	// Calculate actual damage
	float ActualDamage = FMath::Max(0.0f, Damage);
	CurrentHealth -= ActualDamage;

	// Clamp health
	CurrentHealth = FMath::Max(0.0f, CurrentHealth);

	// Broadcast health changed
	OnHealthChanged.Broadcast(GetHealthPercent());

	// Check if dead
	if (CurrentHealth <= 0.0f)
	{
		Die(DamageCauser);
	}

	return ActualDamage;
}

void UHealthComponent::Heal(float Amount)
{
	if (bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Min(CurrentHealth + Amount, MaxHealth);
	OnHealthChanged.Broadcast(GetHealthPercent());
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
	bIsDead = false;
	OnHealthChanged.Broadcast(1.0f);
}

void UHealthComponent::SetHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	bIsDead = (CurrentHealth <= 0.0f);
	OnHealthChanged.Broadcast(GetHealthPercent());
	
	UE_LOG(LogTemp, Display, TEXT(">>> HEALTH: SetHealth called - Health set to %.1f/%.1f"), CurrentHealth, MaxHealth);
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentHealth / MaxHealth;
}

void UHealthComponent::Die(AActor* Killer)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	CurrentHealth = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("HealthComponent: %s died"), *GetOwner()->GetName());

	// Broadcast death event
	OnDeath.Broadcast(Killer);
}
