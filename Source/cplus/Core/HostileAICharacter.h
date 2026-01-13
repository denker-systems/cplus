#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "QuestKillable.h"
#include "HostileAICharacter.generated.h"

/**
 * Hostile AI Character
 * 
 * Base class for enemy AI characters
 * Provides: Combat AI, Weapon system, Quest kill tracking
 */
UCLASS(Abstract)
class CPLUS_API AHostileAICharacter : public ABaseAICharacter, public IQuestKillable
{
	GENERATED_BODY()

public:
	AHostileAICharacter();

protected:
	virtual void BeginPlay() override;

public:
	// IQuestKillable interface
	virtual void OnKilledForQuest_Implementation(AActor* Killer) override;

protected:
	virtual void Die(AActor* Killer) override;
};
