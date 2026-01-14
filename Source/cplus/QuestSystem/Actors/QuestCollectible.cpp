#include "QuestCollectible.h"
#include "QuestSubSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

AQuestCollectible::AQuestCollectible()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Block visibility raycast
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // Also block pawn channel for interaction

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
	PromptText->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Above the item
	PromptText->SetWorldSize(40.0f);
	PromptText->SetTextRenderColor(FColor::Yellow);
	PromptText->SetHorizontalAlignment(EHTA_Center);
	PromptText->SetVerticalAlignment(EVRTA_TextCenter);
	PromptText->SetVisibility(false); // Hidden by default
}

void AQuestCollectible::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ========== BeginPlay =========="));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Actor Name: %s"), *GetName());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ItemID: %s"), *ItemID.ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ItemTags: %d tags"), ItemTags.Num());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Interaction Radius: %.1f"), InteractionSphere->GetScaledSphereRadius());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Implements IQuestInteractable: YES"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ============================="));
}

bool AQuestCollectible::CanInteract_Implementation(AActor* Interactor) const
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: CanInteract called"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Actor: %s"), *GetName());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Interactor: %s"), Interactor ? *Interactor->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: bIsCollected: %s"), bIsCollected ? TEXT("true") : TEXT("false"));
	
	bool bCanInteract = !bIsCollected;
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Result: %s"), bCanInteract ? TEXT("CAN INTERACT") : TEXT("CANNOT INTERACT"));
	
	return bCanInteract;
}

void AQuestCollectible::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ========== Interact_Implementation =========="));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Actor: %s"), *GetName());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Interactor: %s"), Interactor ? *Interactor->GetName() : TEXT("NULL"));
	
	if (!CanInteract_Implementation(Interactor))
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST COLLECTIBLE: Cannot interact - already collected"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Calling Collect()"));
	Collect(Interactor);
}

void AQuestCollectible::Collect(AActor* Collector)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ========== Collect =========="));
	
	if (bIsCollected)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> QUEST COLLECTIBLE: Already collected!"));
		return;
	}
	
	if (!Collector)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COLLECTIBLE: Collector is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Collecting item"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ItemID: %s"), *ItemID.ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ItemTags: %d"), ItemTags.Num());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Collector: %s"), *Collector->GetName());

	bIsCollected = true;

	// Notify QuestSubsystem
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Getting QuestSubsystem..."));
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COLLECTIBLE: GameInstance is NULL!"));
		return;
	}
	
	UQuestSubSystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST COLLECTIBLE: QuestSubsystem is NULL!"));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: QuestSubsystem found!"));
	
	// Build event tags for quest system
	FGameplayTagContainer EventTags = ItemTags;
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: EventTags count: %d"), EventTags.Num());
	
	// Notify quest system about collection
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Calling QuestSubsystem->NotifyQuestEvent"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: QuestID: NAME_None (will check all quests)"));
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: EventTags: %d tags"), EventTags.Num());
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Instigator: %s"), *Collector->GetName());
	
	QuestSubsystem->NotifyQuestEvent(NAME_None, EventTags, Collector);
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: NotifyQuestEvent complete"));

	// Play effects
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Playing effects..."));
	if (CollectEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectEffect, GetActorLocation());
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Spawned particle effect"));
	}

	if (CollectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSound, GetActorLocation());
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Played sound"));
	}

	// Call Blueprint event
	OnCollected(Collector);

	// Hide the collectible
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Hiding actor..."));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// Respawn if configured
	if (bRespawns)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Will respawn in %.1f seconds"), RespawnDelay);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AQuestCollectible::Respawn, RespawnDelay, false);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Destroying actor (no respawn)"));
		Destroy();
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: ========== Collect Complete =========="));
}

void AQuestCollectible::Respawn()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Respawning [%s]"), *GetName());

	bIsCollected = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AQuestCollectible::ShowPrompt_Implementation()
{
	if (PromptText)
	{
		PromptText->SetText(InteractionPrompt);
		PromptText->SetVisibility(true);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Showing prompt: %s"), *InteractionPrompt.ToString());
	}
}

void AQuestCollectible::HidePrompt_Implementation()
{
	if (PromptText)
	{
		PromptText->SetVisibility(false);
		UE_LOG(LogTemp, Display, TEXT(">>> QUEST COLLECTIBLE: Hiding prompt"));
	}
}
