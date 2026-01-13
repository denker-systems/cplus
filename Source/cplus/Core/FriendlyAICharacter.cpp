#include "FriendlyAICharacter.h"
#include "QuestGiverComponent.h"

AFriendlyAICharacter::AFriendlyAICharacter()
{
	// Set team to friendly
	Team = EAITeam::Friendly;

	// Create quest giver component
	QuestGiver = CreateDefaultSubobject<UQuestGiverComponent>(TEXT("QuestGiver"));
}

void AFriendlyAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFriendlyAICharacter::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("FriendlyAICharacter: %s interacted with by %s"), 
		*GetName(), *Interactor->GetName());

	// Quest giver component handles quest offering
	if (QuestGiver)
	{
		// QuestGiver->OfferQuests(Interactor);
	}
}
