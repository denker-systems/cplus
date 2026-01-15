#include "QuestInteractableObject.h"
#include "QuestSubSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

AQuestInteractableObject::AQuestInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	// Create interaction sphere
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(200.0f);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Create text render component
	PromptText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PromptText"));
	PromptText->SetupAttachment(RootComponent);
	PromptText->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	PromptText->SetWorldSize(40.0f);
	PromptText->SetTextRenderColor(FColor::Cyan);
	PromptText->SetHorizontalAlignment(EHTA_Center);
	PromptText->SetVerticalAlignment(EVRTA_TextCenter);
	PromptText->SetVisibility(false);
}

void AQuestInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST INTERACTABLE: Spawned [%s]"), *GetName());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST INTERACTABLE: ItemID=%s, Tag=%s"), 
		*ItemID.ToString(), *InteractionTag.ToString());
}

bool AQuestInteractableObject::CanInteract_Implementation(AActor* Interactor) const
{
	// Can interact if:
	// - Not yet interacted, OR
	// - Can interact multiple times
	return !bHasBeenInteracted || bCanInteractMultipleTimes;
}

void AQuestInteractableObject::Interact_Implementation(AActor* Interactor)
{
	if (!CanInteract_Implementation(Interactor))
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST INTERACTABLE: Cannot interact with [%s] - already used"), *GetName());
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST INTERACTABLE: Interacting with [%s] (Tag=%s)"), 
		*GetName(), *InteractionTag.ToString());

	bHasBeenInteracted = true;

	// Notify QuestSubsystem (same pattern as QuestCollectible)
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
		if (QuestSubsystem)
		{
			// Build event tags from InteractionTag
			FGameplayTagContainer EventTags;
			if (InteractionTag.IsValid())
			{
				EventTags.AddTag(InteractionTag);
			}

			UE_LOG(LogTemp, Display, TEXT(">>> QUEST INTERACTABLE: Notifying QuestSubsystem with %d tag(s)"), EventTags.Num());
			QuestSubsystem->NotifyQuestEvent(NAME_None, EventTags, Interactor);
		}
	}

	// Play effects
	if (InteractEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InteractEffect, GetActorLocation());
	}

	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());
	}

	// Call Blueprint event
	OnInteracted(Interactor);
}

void AQuestInteractableObject::ShowPrompt_Implementation()
{
	if (PromptText)
	{
		PromptText->SetText(InteractionPrompt);
		PromptText->SetVisibility(true);
	}
}

void AQuestInteractableObject::HidePrompt_Implementation()
{
	if (PromptText)
	{
		PromptText->SetVisibility(false);
	}
}
