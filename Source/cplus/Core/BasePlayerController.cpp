// Copyright Epic Games, Inc. All Rights Reserved.

#include "BasePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Input/SVirtualJoystick.h"

ABasePlayerController::ABasePlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: Constructor called"));
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: BeginPlay called"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: IsLocalPlayerController = %s"), IsLocalPlayerController() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: Pawn = %s"), GetPawn() ? *GetPawn()->GetName() : TEXT("NULL"));

	// Only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// Spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// Add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);
		}
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: SetupInputComponent called"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: IsLocalPlayerController = %s"), IsLocalPlayerController() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: DefaultMappingContexts.Num() = %d"), DefaultMappingContexts.Num());

	// Only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: Enhanced Input Subsystem found"));
			
			// Add default mapping contexts
			int32 ContextIndex = 0;
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				if (CurrentContext)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
					UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: Added mapping context %d: %s"), ContextIndex, *CurrentContext->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("BasePlayerController: Mapping context %d is NULL!"), ContextIndex);
				}
				ContextIndex++;
			}

			// Only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					if (CurrentContext)
					{
						Subsystem->AddMappingContext(CurrentContext, 0);
						UE_LOG(LogTemp, Warning, TEXT("BasePlayerController: Added mobile excluded context: %s"), *CurrentContext->GetName());
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BasePlayerController: Enhanced Input Subsystem NOT FOUND!"));
		}
	}
}

bool ABasePlayerController::ShouldUseTouchControls() const
{
	// Are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
