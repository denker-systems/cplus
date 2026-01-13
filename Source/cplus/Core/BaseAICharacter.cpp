#include "BaseAICharacter.h"
#include "QuestTargetComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	// Create quest target component
	QuestTarget = CreateDefaultSubobject<UQuestTargetComponent>(TEXT("QuestTarget"));
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

float ABaseAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= ActualDamage;

	if (CurrentHP <= 0.0f)
	{
		CurrentHP = 0.0f;
		Die(DamageCauser);
	}

	return ActualDamage;
}

void ABaseAICharacter::Die(AActor* Killer)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	UE_LOG(LogTemp, Log, TEXT("BaseAICharacter: %s died"), *GetName());

	// Broadcast death event
	OnAIDeath.Broadcast(Killer);

	// Subclasses implement specific death behavior (ragdoll, etc.)
}
