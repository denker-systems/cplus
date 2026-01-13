// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerCharacter.h"
#include "Logging/LogMacros.h"
#include "cplusCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 * A basic first person character
 * 
 * Legacy class - all functionality now in BasePlayerCharacter
 * Kept for backward compatibility with existing Blueprints
 */
UCLASS(abstract)
class AcplusCharacter : public ABasePlayerCharacter
{
	GENERATED_BODY()

public:
	AcplusCharacter();
};

