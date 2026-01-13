// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseGameMode.h"
#include "BasePlayerCharacter.h"
#include "BasePlayerController.h"
#include "UObject/ConstructorHelpers.h"

ABaseGameMode::ABaseGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseGameMode: Constructor called"));
	
	// Set default pawn class to BasePlayerCharacter
	DefaultPawnClass = ABasePlayerCharacter::StaticClass();
	
	// Set default player controller class to BasePlayerController
	PlayerControllerClass = ABasePlayerController::StaticClass();
	
	UE_LOG(LogTemp, Warning, TEXT("BaseGameMode: DefaultPawnClass = %s"), DefaultPawnClass ? *DefaultPawnClass->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("BaseGameMode: PlayerControllerClass = %s"), PlayerControllerClass ? *PlayerControllerClass->GetName() : TEXT("NULL"));
}
