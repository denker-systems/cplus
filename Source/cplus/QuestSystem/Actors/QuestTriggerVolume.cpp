#include "QuestTriggerVolume.h"
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
			UE_LOG(LogTemp, Log, TEXT("QuestTriggerVolume: Player entered trigger for quest %s, stage %s"), 
				*AssociatedQuestID.ToString(), *TriggerStageID.ToString());

			OnTriggerActivated.Broadcast(OtherActor);
		}
	}
}
