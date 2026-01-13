// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "cplusCharacter.h"
#include "HorrorCharacter.generated.h"

class USpotLightComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChangedDelegate, bool, bSprinting);

/**
 *  Simple first person horror character
 *  Provides stamina-based sprinting
 */
UCLASS(abstract)
class CPLUS_API AHorrorCharacter : public AcplusCharacter
{
	GENERATED_BODY()

	// SpotLight, SprintAction, sprint/stamina system now in BasePlayerCharacter
	// (via AcplusCharacter → ABasePlayerCharacter)

protected:

public:

	// Sprint delegates now in BasePlayerCharacter

protected:

	/** Constructor */
	AHorrorCharacter();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

protected:

	// DoStartSprint, DoEndSprint, SprintFixedTick now in BasePlayerCharacter
};
