#include "QuestMarker.h"
#include "Components/StaticMeshComponent.h"

AQuestMarker::AQuestMarker()
{
	PrimaryActorTick.bCanEverTick = false;

	// Billboard for editor
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	// Marker mesh
	MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
	MarkerMesh->SetupAttachment(RootComponent);
	MarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AQuestMarker::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsVisible)
	{
		HideMarker();
	}
}

void AQuestMarker::ShowMarker()
{
	bIsVisible = true;
	MarkerMesh->SetVisibility(true);
}

void AQuestMarker::HideMarker()
{
	bIsVisible = false;
	MarkerMesh->SetVisibility(false);
}
