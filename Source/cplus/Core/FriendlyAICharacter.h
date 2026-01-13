#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "FriendlyAICharacter.generated.h"

/**
 * Friendly AI Character
 * 
 * Simple variant of BaseAICharacter with Team set to Friendly
 * All functionality inherited from BaseAICharacter
 */
UCLASS(Abstract)
class CPLUS_API AFriendlyAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	AFriendlyAICharacter();
};
