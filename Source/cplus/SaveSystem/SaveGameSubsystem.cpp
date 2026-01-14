#include "SaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/Runtime/QuestSubSystem.h"
#include "Core/PlayerProgressionComponent.h"
#include "Core/HealthComponent.h"
#include "Core/GameDataSubsystem.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/ItemDefinition.h"
#include "Core/BasePlayerCharacter.h"
#include "GameFramework/PlayerController.h"

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogTemp, Log, TEXT("SaveGameSubsystem initialized"));
}

FString USaveGameSubsystem::GetSaveSlotName(int32 SlotIndex) const
{
	return FString::Printf(TEXT("%s_%d"), SaveSlotPrefix, SlotIndex);
}

void USaveGameSubsystem::NewGame(int32 SlotIndex, const FString& PlayerName)
{
	UE_LOG(LogTemp, Display, TEXT("=== NEW GAME START ==="));
	UE_LOG(LogTemp, Display, TEXT("Slot: %d, Player: %s"), SlotIndex, *PlayerName);

	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid save slot: %d"), SlotIndex);
		return;
	}

	// Reset all game systems
	ResetGameState();

	// Create new save data
	CurrentSaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
	if (!CurrentSaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
		return;
	}

	// Initialize save metadata
	CurrentSaveData->SlotName = GetSaveSlotName(SlotIndex);
	CurrentSaveData->SlotIndex = SlotIndex;
	CurrentSaveData->PlayerName = PlayerName;
	CurrentSaveData->SaveTime = FDateTime::Now();
	CurrentSaveData->PlayTimeSeconds = 0.0f;
	CurrentSaveData->CurrentLevel = UGameplayStatics::GetCurrentLevelName(this);

	// Set as current slot
	CurrentSaveSlot = SlotIndex;

	// Save initial state
	SaveGame(SlotIndex);

	UE_LOG(LogTemp, Display, TEXT("=== NEW GAME COMPLETE ==="));
}

bool USaveGameSubsystem::SaveGame(int32 SlotIndex)
{
	UE_LOG(LogTemp, Display, TEXT("=== SAVE GAME START ==="));
	UE_LOG(LogTemp, Display, TEXT("Slot: %d"), SlotIndex);

	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid save slot: %d"), SlotIndex);
		return false;
	}

	// Create or update save data
	if (!CurrentSaveData || CurrentSaveData->SlotIndex != SlotIndex)
	{
		CurrentSaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
		if (!CurrentSaveData)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
			return false;
		}
		CurrentSaveData->SlotName = GetSaveSlotName(SlotIndex);
		CurrentSaveData->SlotIndex = SlotIndex;
	}

	// Update metadata
	CurrentSaveData->SaveTime = FDateTime::Now();
	CurrentSaveData->CurrentLevel = UGameplayStatics::GetCurrentLevelName(this);

	// Collect game state
	CollectSaveData(CurrentSaveData);

	// Save to disk
	FString SlotName = GetSaveSlotName(SlotIndex);
	bool bSuccess = UGameplayStatics::SaveGameToSlot(CurrentSaveData, SlotName, 0);

	if (bSuccess)
	{
		CurrentSaveSlot = SlotIndex;
		UE_LOG(LogTemp, Display, TEXT("Game saved successfully to slot %d"), SlotIndex);
		UE_LOG(LogTemp, Display, TEXT("Quests: %d, Level: %d, Gold: %d"), 
			CurrentSaveData->ActiveQuests.Num(),
			CurrentSaveData->PlayerProgression.Level,
			CurrentSaveData->PlayerProgression.Gold);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot %d"), SlotIndex);
	}

	UE_LOG(LogTemp, Display, TEXT("=== SAVE GAME END ==="));
	return bSuccess;
}

bool USaveGameSubsystem::QuickSave()
{
	if (CurrentSaveSlot < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No active save slot for quick save"));
		return false;
	}

	return SaveGame(CurrentSaveSlot);
}

bool USaveGameSubsystem::LoadGame(int32 SlotIndex)
{
	UE_LOG(LogTemp, Display, TEXT("=== LOAD GAME START ==="));
	UE_LOG(LogTemp, Display, TEXT("Slot: %d"), SlotIndex);

	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid save slot: %d"), SlotIndex);
		return false;
	}

	// Check if save exists
	FString SlotName = GetSaveSlotName(SlotIndex);
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save does not exist in slot %d"), SlotIndex);
		return false;
	}

	// Load save data
	USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	CurrentSaveData = Cast<UGameSaveData>(LoadedSave);

	if (!CurrentSaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load save from slot %d"), SlotIndex);
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Save loaded: %s"), *CurrentSaveData->PlayerName);
	UE_LOG(LogTemp, Display, TEXT("Level: %d, Quests: %d"), 
		CurrentSaveData->PlayerProgression.Level,
		CurrentSaveData->ActiveQuests.Num());

	// Apply save data to game
	ApplySaveData(CurrentSaveData);

	CurrentSaveSlot = SlotIndex;

	UE_LOG(LogTemp, Display, TEXT("=== LOAD GAME COMPLETE ==="));
	return true;
}

bool USaveGameSubsystem::Continue()
{
	int32 MostRecentSlot = GetMostRecentSaveSlot();
	if (MostRecentSlot < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No saves found to continue"));
		return false;
	}

	return LoadGame(MostRecentSlot);
}

bool USaveGameSubsystem::DoesSaveExist(int32 SlotIndex) const
{
	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		return false;
	}

	FString SlotName = GetSaveSlotName(SlotIndex);
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

UGameSaveData* USaveGameSubsystem::GetSaveMetadata(int32 SlotIndex) const
{
	if (!DoesSaveExist(SlotIndex))
	{
		return nullptr;
	}

	FString SlotName = GetSaveSlotName(SlotIndex);
	USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	return Cast<UGameSaveData>(LoadedSave);
}

int32 USaveGameSubsystem::GetMostRecentSaveSlot() const
{
	int32 MostRecentSlot = -1;
	FDateTime MostRecentTime = FDateTime::MinValue();

	for (int32 i = 0; i < MaxSaveSlots; ++i)
	{
		if (UGameSaveData* SaveData = GetSaveMetadata(i))
		{
			if (SaveData->SaveTime > MostRecentTime)
			{
				MostRecentTime = SaveData->SaveTime;
				MostRecentSlot = i;
			}
		}
	}

	return MostRecentSlot;
}

bool USaveGameSubsystem::DeleteSave(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
	{
		return false;
	}

	FString SlotName = GetSaveSlotName(SlotIndex);
	bool bSuccess = UGameplayStatics::DeleteGameInSlot(SlotName, 0);

	if (bSuccess && CurrentSaveSlot == SlotIndex)
	{
		CurrentSaveSlot = -1;
		CurrentSaveData = nullptr;
	}

	return bSuccess;
}

void USaveGameSubsystem::CollectSaveData(UGameSaveData* SaveData)
{
	if (!SaveData)
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Collecting save data..."));

	// Get player character
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABasePlayerCharacter* Player = PC ? Cast<ABasePlayerCharacter>(PC->GetPawn()) : nullptr;

	// Save quest progress
	if (UQuestSubSystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubSystem>())
	{
		SaveData->ActiveQuests.Empty();
		
		TArray<FActiveQuest> ActiveQuests = QuestSub->GetActiveQuests();
		for (const FActiveQuest& ActiveQuest : ActiveQuests)
		{
			if (!ActiveQuest.QuestDefinition)
			{
				continue;
			}

			FQuestSaveData QuestSave;
			QuestSave.QuestID = ActiveQuest.QuestDefinition->QuestID;
			QuestSave.State = (uint8)ActiveQuest.State;
			QuestSave.CurrentObjectiveIndex = ActiveQuest.CurrentObjectiveIndex;
			QuestSave.AcceptedTime = ActiveQuest.AcceptedTime;

			// Save task progress
			if (ActiveQuest.CurrentObjectiveIndex < ActiveQuest.QuestDefinition->Objectives.Num())
			{
				const FQuestObjective& Objective = ActiveQuest.QuestDefinition->Objectives[ActiveQuest.CurrentObjectiveIndex];
				for (UQuestTask* Task : Objective.Tasks)
				{
					if (Task)
					{
						QuestSave.TaskProgress.Add(Task->CurrentCount);
					}
				}
			}

			SaveData->ActiveQuests.Add(QuestSave);
		}

		UE_LOG(LogTemp, Display, TEXT("Saved %d quest(s)"), SaveData->ActiveQuests.Num());
	}

	// Save player progression
	if (Player && Player->GetComponentByClass(UPlayerProgressionComponent::StaticClass()))
	{
		UPlayerProgressionComponent* Progression = Cast<UPlayerProgressionComponent>(
			Player->GetComponentByClass(UPlayerProgressionComponent::StaticClass()));

		if (Progression)
		{
			SaveData->PlayerProgression.Level = Progression->CurrentLevel;
			SaveData->PlayerProgression.Experience = Progression->CurrentExperience;
			SaveData->PlayerProgression.Gold = Progression->Gold;
			SaveData->PlayerProgression.FactionReputation = Progression->FactionReputation;

			UE_LOG(LogTemp, Display, TEXT("Saved progression: Level %d, XP %d, Gold %d"),
				SaveData->PlayerProgression.Level,
				SaveData->PlayerProgression.Experience,
				SaveData->PlayerProgression.Gold);
		}
	}

	// Save inventory
	if (Player && Player->GetInventory())
	{
		UInventoryComponent* Inventory = Player->GetInventory();
		SaveData->InventoryItems.Empty();

		for (const FInventoryItem& Item : Inventory->Items)
		{
			if (Item.ItemData)
			{
				FInventoryItemSaveData ItemSave;
				ItemSave.ItemID = Item.ItemData->ItemID;
				ItemSave.Quantity = Item.Quantity;
				ItemSave.InstanceID = Item.InstanceID;

				SaveData->InventoryItems.Add(ItemSave);
			}
		}

		UE_LOG(LogTemp, Display, TEXT("Saved %d inventory item(s)"), SaveData->InventoryItems.Num());
	}

	// Save player state
	if (Player)
	{
		SaveData->PlayerState.Location = Player->GetActorLocation();
		SaveData->PlayerState.Rotation = Player->GetActorRotation();

		if (UHealthComponent* Health = Player->GetHealthComponent())
		{
			SaveData->PlayerState.Health = Health->GetCurrentHealth();
			SaveData->PlayerState.MaxHealth = Health->GetMaxHealth();
		}

		UE_LOG(LogTemp, Display, TEXT("Saved player state at location: %s"), 
			*SaveData->PlayerState.Location.ToString());
	}
}

void USaveGameSubsystem::ApplySaveData(UGameSaveData* SaveData)
{
	if (!SaveData)
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Applying save data..."));

	// Reset game state first
	ResetGameState();

	// Get player character
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABasePlayerCharacter* Player = PC ? Cast<ABasePlayerCharacter>(PC->GetPawn()) : nullptr;

	// Restore quest progress
	if (UQuestSubSystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubSystem>())
	{
		for (const FQuestSaveData& QuestSave : SaveData->ActiveQuests)
		{
			UE_LOG(LogTemp, Display, TEXT("Restoring quest: %s"), *QuestSave.QuestID.ToString());
			QuestSub->LoadQuestProgress(
				QuestSave.QuestID,
				QuestSave.State,
				QuestSave.CurrentObjectiveIndex,
				QuestSave.TaskProgress,
				QuestSave.AcceptedTime
			);
		}
		UE_LOG(LogTemp, Display, TEXT("Restored %d quest(s)"), SaveData->ActiveQuests.Num());
	}

	// Restore player progression
	if (Player && Player->GetComponentByClass(UPlayerProgressionComponent::StaticClass()))
	{
		UPlayerProgressionComponent* Progression = Cast<UPlayerProgressionComponent>(
			Player->GetComponentByClass(UPlayerProgressionComponent::StaticClass()));

		if (Progression)
		{
			Progression->CurrentLevel = SaveData->PlayerProgression.Level;
			Progression->CurrentExperience = SaveData->PlayerProgression.Experience;
			Progression->Gold = SaveData->PlayerProgression.Gold;
			Progression->FactionReputation = SaveData->PlayerProgression.FactionReputation;

			UE_LOG(LogTemp, Display, TEXT("Restored progression: Level %d, XP %d, Gold %d"),
				Progression->CurrentLevel,
				Progression->CurrentExperience,
				Progression->Gold);
		}
	}

	// Restore inventory
	if (Player && Player->GetInventory())
	{
		UInventoryComponent* Inventory = Player->GetInventory();
		UGameDataSubsystem* GameData = GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
		
		if (GameData)
		{
			for (const FInventoryItemSaveData& ItemSave : SaveData->InventoryItems)
			{
				// Load item definition from GameDataSubsystem
				UItemDefinition* ItemDef = GameData->GetItemByID(ItemSave.ItemID);
				if (ItemDef)
				{
					Inventory->AddItem(ItemDef, ItemSave.Quantity);
					UE_LOG(LogTemp, Display, TEXT("Restored item: %s x%d"), *ItemSave.ItemID.ToString(), ItemSave.Quantity);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Item definition not found: %s"), *ItemSave.ItemID.ToString());
				}
			}
			UE_LOG(LogTemp, Display, TEXT("Restored %d inventory item(s)"), SaveData->InventoryItems.Num());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameDataSubsystem not found - cannot restore inventory"));
		}
	}

	// Restore player state
	if (Player)
	{
		Player->SetActorLocation(SaveData->PlayerState.Location);
		Player->SetActorRotation(SaveData->PlayerState.Rotation);

		if (UHealthComponent* Health = Player->GetHealthComponent())
		{
			Health->SetHealth(SaveData->PlayerState.Health);
			UE_LOG(LogTemp, Display, TEXT("Restored health: %.1f/%.1f"),
				SaveData->PlayerState.Health,
				SaveData->PlayerState.MaxHealth);
		}
	}
}

void USaveGameSubsystem::ResetGameState()
{
	UE_LOG(LogTemp, Display, TEXT("Resetting game state..."));

	// Reset quest system
	if (UQuestSubSystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubSystem>())
	{
		QuestSub->ResetAllQuests();
	}

	// Reset player progression
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABasePlayerCharacter* Player = PC ? Cast<ABasePlayerCharacter>(PC->GetPawn()) : nullptr;

	if (Player)
	{
		if (UPlayerProgressionComponent* Progression = Cast<UPlayerProgressionComponent>(
			Player->GetComponentByClass(UPlayerProgressionComponent::StaticClass())))
		{
			Progression->CurrentLevel = 1;
			Progression->CurrentExperience = 0;
			Progression->Gold = 0;
			Progression->FactionReputation.Empty();
		}

		// Reset inventory
		if (UInventoryComponent* Inventory = Player->GetInventory())
		{
			Inventory->Items.Empty();
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Game state reset complete"));
}
