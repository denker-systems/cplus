#include "MainMenuActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AMainMenuActor::AMainMenuActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create scene root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	// Create camera component
	MenuCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MenuCamera"));
	MenuCamera->SetupAttachment(SceneRoot);
	MenuCamera->SetFieldOfView(CameraFOV);

	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Constructor called"));
}

void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: BeginPlay called"));
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Location: %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Rotation: %s"), *GetActorRotation().ToString());
}

void AMainMenuActor::ActivateMenuCamera()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: ActivateMenuCamera called"));

	if (!bActivateCameraOnShow)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Camera activation disabled"));
		return;
	}

	// Get player controller
	CachedPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!CachedPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> MAIN MENU ACTOR: No player controller found"));
		return;
	}

	// Get current view target
	AActor* CurrentViewTarget = CachedPlayerController->GetViewTarget();
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Current view target: %s"), 
		CurrentViewTarget ? *CurrentViewTarget->GetName() : TEXT("NULL"));

	// Only store original view target if it's not already this actor
	if (CurrentViewTarget != this)
	{
		OriginalViewTarget = CurrentViewTarget;
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Stored original view target: %s"), 
			OriginalViewTarget ? *OriginalViewTarget->GetName() : TEXT("NULL"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Already viewing this actor, keeping previous original target"));
	}

	// Set view target to this actor's camera (even if already set, to ensure blend)
	CachedPlayerController->SetViewTargetWithBlend(this, 0.5f);
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Camera activated with 0.5s blend"));
}

void AMainMenuActor::DeactivateMenuCamera()
{
	UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: DeactivateMenuCamera called"));

	if (!CachedPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> MAIN MENU ACTOR: No cached player controller"));
		return;
	}

	// Restore original view target
	if (OriginalViewTarget)
	{
		CachedPlayerController->SetViewTargetWithBlend(OriginalViewTarget, 0.5f);
		UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Restored view target: %s"), 
			*OriginalViewTarget->GetName());
	}
	else
	{
		// No original view target, reset to player pawn
		if (APawn* PlayerPawn = CachedPlayerController->GetPawn())
		{
			CachedPlayerController->SetViewTargetWithBlend(PlayerPawn, 0.5f);
			UE_LOG(LogTemp, Display, TEXT(">>> MAIN MENU ACTOR: Restored view to player pawn"));
		}
	}

	// Clear cached references
	CachedPlayerController = nullptr;
	OriginalViewTarget = nullptr;
}
