#include "QuestGiverNPC.h"
#include "QuestSystem/Components/QuestGiverComponent.h"

AQuestGiverNPC::AQuestGiverNPC()
{
	// Set team to friendly (previously inherited from FriendlyAICharacter)
	Team = EAITeam::Friendly;
	TeamByte = 0;

	// Create QuestGiver component (only for quest-giving NPCs)
	QuestGiver = CreateDefaultSubobject<UQuestGiverComponent>(TEXT("QuestGiver"));
}

void AQuestGiverNPC::BeginPlay()
{
	Super::BeginPlay();
}
