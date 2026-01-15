#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "QuestGiverNPC.generated.h"

/**
 * Quest Giver NPC
 * 
 * Specialized friendly NPC for quest giving
 * Inherits from ABaseAICharacter with Team=Friendly
 */
UCLASS()
class CPLUS_API AQuestGiverNPC : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	AQuestGiverNPC();

protected:
	virtual void BeginPlay() override;
};
