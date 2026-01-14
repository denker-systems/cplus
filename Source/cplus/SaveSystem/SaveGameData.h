#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

/**
 * Quest Save Data
 * Stores progress for a single quest
 */
USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()

	/** Quest identifier */
	UPROPERTY()
	FName QuestID;

	/** Quest state (0=NotStarted, 1=Active, 2=Completed, 3=Failed) */
	UPROPERTY()
	uint8 State = 0;

	/** Current objective index */
	UPROPERTY()
	int32 CurrentObjectiveIndex = 0;

	/** Task progress for each task (CurrentCount values) */
	UPROPERTY()
	TArray<int32> TaskProgress;

	/** When quest was accepted */
	UPROPERTY()
	FDateTime AcceptedTime;
};

/**
 * Inventory Item Save Data
 * Stores a single inventory item
 */
USTRUCT(BlueprintType)
struct FInventoryItemSaveData
{
	GENERATED_BODY()

	/** Item identifier */
	UPROPERTY()
	FName ItemID;

	/** Item quantity */
	UPROPERTY()
	int32 Quantity = 0;

	/** Unique instance ID */
	UPROPERTY()
	FGuid InstanceID;
};

/**
 * Player Progression Save Data
 * Stores player level, XP, gold, reputation
 */
USTRUCT(BlueprintType)
struct FPlayerProgressionSaveData
{
	GENERATED_BODY()

	/** Player level */
	UPROPERTY()
	int32 Level = 1;

	/** Current experience points */
	UPROPERTY()
	int32 Experience = 0;

	/** Gold amount */
	UPROPERTY()
	int32 Gold = 0;

	/** Faction reputation values */
	UPROPERTY()
	TMap<FName, int32> FactionReputation;
};

/**
 * Player State Save Data
 * Stores player location, rotation, health
 */
USTRUCT(BlueprintType)
struct FPlayerStateSaveData
{
	GENERATED_BODY()

	/** Player world location */
	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	/** Player rotation */
	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	/** Current health */
	UPROPERTY()
	float Health = 100.0f;

	/** Max health */
	UPROPERTY()
	float MaxHealth = 100.0f;
};

/**
 * Complete Save Game Data
 * Main save game class containing all persistent data
 */
UCLASS()
class CPLUS_API UGameSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	/** Save slot name (e.g., "SaveSlot_0") */
	UPROPERTY()
	FString SlotName;

	/** Save slot index (0, 1, 2) */
	UPROPERTY()
	int32 SlotIndex = 0;

	/** When this save was created */
	UPROPERTY()
	FDateTime SaveTime;

	/** Player name/character name */
	UPROPERTY()
	FString PlayerName = TEXT("Player");

	/** Current level/map name */
	UPROPERTY()
	FString CurrentLevel;

	/** Play time in seconds */
	UPROPERTY()
	float PlayTimeSeconds = 0.0f;

	// ===== GAME DATA =====

	/** Quest progress */
	UPROPERTY()
	TArray<FQuestSaveData> ActiveQuests;

	/** Player progression (level, XP, gold) */
	UPROPERTY()
	FPlayerProgressionSaveData PlayerProgression;

	/** Inventory items */
	UPROPERTY()
	TArray<FInventoryItemSaveData> InventoryItems;

	/** Player state (location, health) */
	UPROPERTY()
	FPlayerStateSaveData PlayerState;

	// ===== METADATA =====

	/** Save file version for compatibility */
	UPROPERTY()
	int32 SaveVersion = 1;
};
