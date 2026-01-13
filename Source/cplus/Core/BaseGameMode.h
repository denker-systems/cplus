// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * Base Game Mode
 * 
 * Simple game mode that uses BasePlayerCharacter and BasePlayerController
 * Provides foundation for all game modes in the project
 */
UCLASS()
class CPLUS_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseGameMode();
};
