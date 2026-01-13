#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "QuestMarker.generated.h"

/**
 * Quest Marker
 * 
 * Visual marker for quest objectives and tracked locations
 */
UCLASS()
class CPLUS_API AQuestMarker : public AActor
{
	GENERATED_BODY()

public:
	AQuestMarker();

protected:
	virtual void BeginPlay() override;

	/** Billboard component for editor visibility */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Marker")
	UBillboardComponent* Billboard;

	/** Marker icon/mesh (visible in-game) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Marker")
	class UStaticMeshComponent* MarkerMesh;

	/** Is marker currently visible? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Marker")
	bool bIsVisible = true;

	/** Marker color */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Marker")
	FLinearColor MarkerColor = FLinearColor::Yellow;

public:
	/** Show marker */
	UFUNCTION(BlueprintCallable, Category = "Quest Marker")
	void ShowMarker();

	/** Hide marker */
	UFUNCTION(BlueprintCallable, Category = "Quest Marker")
	void HideMarker();
};
