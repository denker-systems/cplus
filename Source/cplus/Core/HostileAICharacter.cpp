#include "HostileAICharacter.h"

AHostileAICharacter::AHostileAICharacter()
{
	// Set team to hostile
	Team = EAITeam::Hostile;
}

void AHostileAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AHostileAICharacter::OnKilledForQuest_Implementation(AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("HostileAICharacter: %s killed for quest by %s"), 
		*GetName(), *Killer->GetName());

	// Notify quest system via QuestTargetComponent
	if (QuestTarget)
	{
		// QuestTarget->NotifyKilled(Killer);
	}
}

void AHostileAICharacter::Die(AActor* Killer)
{
	Super::Die(Killer);

	// Notify quest system
	OnKilledForQuest_Implementation(Killer);

	// Subclasses implement ragdoll, loot drop, etc.
}
