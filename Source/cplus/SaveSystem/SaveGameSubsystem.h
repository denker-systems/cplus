#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameData.h"
#include "SaveGameSubsystem.generated.h"

/**
 * Save Game Subsystem
 * 
 * Manages save/load operations with multiple save slots.
 * Handles New Game, Save Game, Load Game functionality.
 * 
 * Features:
 * - 3 save slots (0, 1, 2)
 * - Auto-save support
 * - Save slot metadata (time, level, playtime)
 */
UCLASS()
class CPLUS_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ===== CONSTANTS =====

	/** Number of save slots available */
	static constexpr int32 MaxSaveSlots = 3;

	/** Save slot name prefix */
	static constexpr const TCHAR* SaveSlotPrefix = TEXT("SaveSlot");

	// ===== NEW GAME =====

	/**
	 * Start a new game in specified slot
	 * Clears all progress and starts fresh
	 * 
	 * @param SlotIndex Save slot to use (0-2)
	 * @param PlayerName Optional player name
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void NewGame(int32 SlotIndex = 0, const FString& PlayerName = TEXT("Player"));

	// ===== SAVE =====

	/**
	 * Save current game state to slot
	 * 
	 * @param SlotIndex Save slot to use (0-2)
	 * @return True if save succeeded
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool SaveGame(int32 SlotIndex);

	/**
	 * Quick save to current slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool QuickSave();

	// ===== LOAD =====

	/**
	 * Load game from slot
	 * 
	 * @param SlotIndex Save slot to load (0-2)
	 * @return True if load succeeded
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool LoadGame(int32 SlotIndex);

	/**
	 * Continue from most recent save
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool Continue();

	// ===== QUERY =====

	/**
	 * Check if save exists in slot
	 * 
	 * @param SlotIndex Save slot to check (0-2)
	 * @return True if save exists
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	bool DoesSaveExist(int32 SlotIndex) const;

	/**
	 * Get save metadata without loading full save
	 * 
	 * @param SlotIndex Save slot to query (0-2)
	 * @return Save data or nullptr if doesn't exist
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	UGameSaveData* GetSaveMetadata(int32 SlotIndex) const;

	/**
	 * Get most recent save slot
	 * @return Slot index or -1 if no saves
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	int32 GetMostRecentSaveSlot() const;

	// ===== DELETE =====

	/**
	 * Delete save in slot
	 * 
	 * @param SlotIndex Save slot to delete (0-2)
	 * @return True if deleted
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool DeleteSave(int32 SlotIndex);

	// ===== CURRENT STATE =====

	/**
	 * Get current active save slot
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	int32 GetCurrentSaveSlot() const { return CurrentSaveSlot; }

	/**
	 * Check if currently in a game session
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	bool IsInGame() const { return CurrentSaveSlot >= 0; }

protected:
	/** Current active save slot (-1 if none) */
	UPROPERTY()
	int32 CurrentSaveSlot = -1;

	/** Current save data (cached) */
	UPROPERTY()
	TObjectPtr<UGameSaveData> CurrentSaveData;

	/** Get save slot name */
	FString GetSaveSlotName(int32 SlotIndex) const;

	/** Collect current game state into save data */
	void CollectSaveData(UGameSaveData* SaveData);

	/** Apply loaded save data to game state */
	void ApplySaveData(UGameSaveData* SaveData);

	/** Reset all game systems to default state */
	void ResetGameState();
};
