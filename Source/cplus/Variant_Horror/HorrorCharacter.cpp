// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/HorrorCharacter.h"
#include "Components/SpotLightComponent.h"

AHorrorCharacter::AHorrorCharacter()
{
	// SpotLight is created in BasePlayerCharacter
	// Enable it in BeginPlay if needed
}

void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Enable spotlight for horror mode
	// Access through GetFirstPersonCameraComponent() and find attached SpotLight
	// Or just leave it disabled by default and enable in Blueprint
}

void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
