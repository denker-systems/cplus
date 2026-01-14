#include "QuestGiverNPC.h"
#include "QuestSystem/Components/QuestGiverComponent.h"

AQuestGiverNPC::AQuestGiverNPC()
{
	// Create QuestGiver component (only for quest-giving NPCs)
	QuestGiver = CreateDefaultSubobject<UQuestGiverComponent>(TEXT("QuestGiver"));
}

void AQuestGiverNPC::BeginPlay()
{
	Super::BeginPlay();
}
