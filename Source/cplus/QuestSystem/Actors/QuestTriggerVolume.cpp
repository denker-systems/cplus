#include "QuestTriggerVolume.h"
#include "QuestSubSystem.h"
#include "GameFramework/Character.h"

AQuestTriggerVolume::AQuestTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	
	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AQuestTriggerVolume::OnTriggerBeginOverlap);
}

void AQuestTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestTriggerVolume::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if player entered
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (Character->IsPlayerControlled())
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST TRIGGER: Player entered location [%s] with Tag=%s"), 
				*GetName(), *LocationTag.ToString());

			// Notify QuestSubsystem (same pattern as QuestInteractableObject)
			UGameInstance* GameInstance = GetWorld()->GetGameInstance();
			if (GameInstance)
			{
				UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
				if (QuestSubsystem)
				{
					// Build event tags from LocationTag
					FGameplayTagContainer EventTags;
					if (LocationTag.IsValid())
					{
						EventTags.AddTag(LocationTag);
					}

					UE_LOG(LogTemp, Display, TEXT(">>> QUEST TRIGGER: Notifying QuestSubsystem with %d tag(s)"), EventTags.Num());
					QuestSubsystem->NotifyQuestEvent(NAME_None, EventTags, OtherActor);
				}
			}

			OnTriggerActivated.Broadcast(OtherActor);
		}
	}
}
