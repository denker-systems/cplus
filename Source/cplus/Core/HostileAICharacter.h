#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "HostileAICharacter.generated.h"

/**
 * Hostile AI Character
 * 
 * Simple variant of BaseAICharacter with Team set to Hostile
 * All functionality inherited from BaseAICharacter
 */
UCLASS(Abstract)
class CPLUS_API AHostileAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	AHostileAICharacter();
};
