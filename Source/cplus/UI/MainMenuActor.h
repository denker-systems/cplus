#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuActor.generated.h"

class UCameraComponent;
class USceneComponent;

/**
 * Main Menu Actor
 * 
 * Placeable actor in level for Main Menu scene setup.
 * Contains camera component for Main Menu view.
 * 
 * Usage:
 * 1. Create Blueprint from this class (BP_MainMenuActor)
 * 2. Place in level where you want Main Menu camera
 * 3. Position and rotate camera for desired Main Menu view
 * 4. Add background meshes/lights around actor for visual scene
 * 
 * Features:
 * - Camera component for Main Menu view
 * - Scene root for organizing background elements
 * - Can be placed anywhere in level
 * - Preview camera view in editor
 */
UCLASS()
class CPLUS_API AMainMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainMenuActor();

	virtual void BeginPlay() override;

	// ===== COMPONENTS =====

	/** Root scene component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	/** Camera for Main Menu view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> MenuCamera;

	// ===== SETTINGS =====

	/** Activate this camera when Main Menu is shown? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
	bool bActivateCameraOnShow = true;

	/** Field of view for menu camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu|Camera", meta = (ClampMin = "5.0", ClampMax = "170.0"))
	float CameraFOV = 90.0f;

	// ===== API =====

	/**
	 * Activate menu camera (called when Main Menu is shown)
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void ActivateMenuCamera();

	/**
	 * Deactivate menu camera (called when Main Menu is hidden)
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void DeactivateMenuCamera();

protected:
	/** Store original camera to restore later */
	UPROPERTY()
	TObjectPtr<class APlayerController> CachedPlayerController;

	UPROPERTY()
	TObjectPtr<AActor> OriginalViewTarget;
};
