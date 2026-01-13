#include "BasePlayerCharacter.h"
#include "QuestTrackerComponent.h"
#include "InventoryComponent.h"
#include "InteractionComponent.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Create player-specific components
	QuestTracker = CreateDefaultSubobject<UQuestTrackerComponent>(TEXT("QuestTracker"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
