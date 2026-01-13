// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTask.h"
#include "QuestTask_Location.generated.h"

/**
 * Quest Task: Reach Location
 * 
 * Tracks player reaching a specific location.
 * Uses radius-based detection for flexible positioning.
 */
UCLASS()
class CPLUS_API UQuestTask_Location : public UQuestTask
{
	GENERATED_BODY()

public:
	UQuestTask_Location();

	/** Target location in world space */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	FVector TargetLocation;

	/** How close player must be to complete (in cm) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task", meta = (ClampMin = "1.0"))
	float AcceptanceRadius = 500.0f;

	/** Should player need to stay in radius for duration? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task")
	bool bRequireStayDuration = false;

	/** How long to stay in radius (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Task", meta = (EditCondition = "bRequireStayDuration", ClampMin = "0.1"))
	float StayDuration = 2.0f;

	/**
	 * Check if a location is within acceptance radius
	 * @param CheckLocation The location to check
	 * @return True if location is within radius
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Task")
	bool IsLocationWithinRadius(const FVector& CheckLocation) const;

	/**
	 * Get distance to target location
	 * @param FromLocation Location to measure from
	 * @return Distance in cm
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Task")
	float GetDistanceToTarget(const FVector& FromLocation) const;
};
