#pragma once

#include "CoreMinimal.h"
#include "FriendlyAICharacter.h"
#include "QuestGiverNPC.generated.h"

/**
 * Quest Giver NPC
 * 
 * Specialized friendly NPC for quest giving
 * Inherits quest giver functionality from AFriendlyAICharacter
 */
UCLASS()
class CPLUS_API AQuestGiverNPC : public AFriendlyAICharacter
{
	GENERATED_BODY()

public:
	AQuestGiverNPC();

protected:
	virtual void BeginPlay() override;
};
