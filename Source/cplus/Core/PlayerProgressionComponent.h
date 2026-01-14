// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerProgressionComponent.generated.h"

/**
 * Delegate for level up events
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, int32, NewLevel, int32, OldLevel);

/**
 * Delegate for experience gained
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceGained, int32, Amount, int32, NewTotal);

/**
 * Delegate for gold changed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGoldChanged, int32, Amount, int32, NewTotal);

/**
 * Player Progression Component
 * 
 * Manages player level, experience, and currency.
 * Integrates with Quest System for reward distribution.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UPlayerProgressionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerProgressionComponent();

	// ===== PROGRESSION DATA =====
	
	/** Current player level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 CurrentLevel = 1;

	/** Current experience points */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 CurrentExperience = 0;

	/** Gold/currency amount */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Gold = 0;

	/** Reputation values per faction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
	TMap<FName, int32> FactionReputation;

	// ===== LEVEL SETTINGS =====
	
	/** Maximum level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression|Settings")
	int32 MaxLevel = 50;

	/** Base XP required for level 2 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression|Settings")
	int32 BaseExperienceRequired = 100;

	/** XP multiplier per level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression|Settings")
	float ExperienceMultiplier = 1.5f;

	// ===== EVENTS =====
	
	UPROPERTY(BlueprintAssignable, Category = "Progression")
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Progression")
	FOnExperienceGained OnExperienceGained;

	UPROPERTY(BlueprintAssignable, Category = "Progression")
	FOnGoldChanged OnGoldChanged;

	// ===== API - EXPERIENCE =====
	
	/**
	 * Add experience points
	 * @param Amount XP to add
	 * @return True if leveled up
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	bool AddExperience(int32 Amount);

	/**
	 * Get XP required for next level
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Progression")
	int32 GetExperienceRequiredForNextLevel() const;

	/**
	 * Get progress to next level (0.0 - 1.0)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Progression")
	float GetLevelProgress() const;

	// ===== API - GOLD =====
	
	/**
	 * Add gold
	 * @param Amount Gold to add
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddGold(int32 Amount);

	/**
	 * Remove gold
	 * @param Amount Gold to remove
	 * @return True if had enough gold
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	bool RemoveGold(int32 Amount);

	/**
	 * Check if has enough gold
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Progression")
	bool HasGold(int32 Amount) const;

	// ===== API - REPUTATION =====
	
	/**
	 * Add reputation with faction
	 * @param FactionID Faction identifier
	 * @param Amount Reputation to add (can be negative)
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddReputation(FName FactionID, int32 Amount);

	/**
	 * Get reputation with faction
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Progression")
	int32 GetReputation(FName FactionID) const;

protected:
	virtual void BeginPlay() override;

	/**
	 * Check for level up and handle it
	 */
	void CheckLevelUp();

	/**
	 * Calculate XP required for specific level
	 */
	int32 CalculateExperienceForLevel(int32 Level) const;
};
