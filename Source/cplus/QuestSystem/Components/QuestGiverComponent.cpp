// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiverComponent.h"
#include "QuestSystem/Runtime/QuestSubSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

// Sets default values for this component's properties
UQuestGiverComponent::UQuestGiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UQuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER COMPONENT: BeginPlay called on instance %p"), this);
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER COMPONENT: Owner = %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("NULL"));
	
	// Auto-load quests from folder if enabled
	if (bAutoLoadQuests && !AutoLoadQuestFolder.IsEmpty())
	{
		LoadQuestsFromFolder();
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER COMPONENT: After load, AvailableQuests.Num() = %d"), AvailableQuests.Num());
}

void UQuestGiverComponent::LoadQuestsFromFolder()
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Auto-loading quests from folder: %s"), *AutoLoadQuestFolder);

	// Get Asset Registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Create filter for QuestDefinition assets in specified folder
	FARFilter Filter;
	Filter.ClassPaths.Add(UQuestDefinition::StaticClass()->GetClassPathName());
	Filter.PackagePaths.Add(FName(*AutoLoadQuestFolder));
	Filter.bRecursivePaths = true;

	// Get assets
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Found %d quest(s) in folder"), AssetDataList.Num());

	// Load each quest
	int32 LoadedCount = 0;
	for (const FAssetData& AssetData : AssetDataList)
	{
		UQuestDefinition* Quest = Cast<UQuestDefinition>(AssetData.GetAsset());
		if (Quest)
		{
			// Check if not already in list
			if (!AvailableQuests.Contains(Quest))
			{
				AvailableQuests.Add(Quest);
				LoadedCount++;
				UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Loaded quest: %s (%s)"), 
					*Quest->UIData.QuestTitle.ToString(), *Quest->QuestID.ToString());
			}
		}
	}

	// Sort quests by QuestID to ensure consistent order
	AvailableQuests.Sort([](const UQuestDefinition& A, const UQuestDefinition& B)
	{
		return A.QuestID.ToString() < B.QuestID.ToString();
	});

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Auto-loaded %d new quest(s), total: %d"), 
		LoadedCount, AvailableQuests.Num());
}

TArray<UQuestDefinition*> UQuestGiverComponent::GetAvailableQuests(AActor* Player)
{
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Called on instance %p"), this);
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Owner = %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: AvailableQuests.Num() = %d"), AvailableQuests.Num());
	
	// Get QuestSubsystem to check quest status
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UQuestSubSystem* QuestSub = GameInstance ? GameInstance->GetSubsystem<UQuestSubSystem>() : nullptr;
	
	TArray<UQuestDefinition*> Result;
	for (int32 i = 0; i < AvailableQuests.Num(); i++)
	{
		UQuestDefinition* Quest = AvailableQuests[i];
		UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Quest[%d] = %p"), i, Quest);
		
		if (!Quest)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> GET AVAILABLE QUESTS: Quest[%d] is NULL!"), i);
			continue;
		}
		
		// Skip if quest is already completed (turned in)
		if (QuestSub && QuestSub->IsQuestCompleted(Quest->QuestID))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Skipping completed quest: %s"), *Quest->QuestID.ToString());
			continue;
		}
		
		// Skip if quest is already active
		if (QuestSub && QuestSub->IsQuestActive(Quest->QuestID))
		{
			UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Skipping active quest: %s"), *Quest->QuestID.ToString());
			continue;
		}
		
		UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Adding quest: %s"), *Quest->QuestID.ToString());
		Result.Add(Quest);
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Returning %d quest(s)"), Result.Num());
	return Result;
}

void UQuestGiverComponent::OfferQuest(UQuestDefinition* Quest, AActor* Player)
{
	if (Quest && Player)
	{
		OnQuestOffered.Broadcast(Quest, Player);
	}
}

void UQuestGiverComponent::HandleInteraction(AActor* Player)
{
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("========================================"));
	UE_LOG(LogTemp, Display, TEXT("QUEST FLOW: HandleInteraction START"));
	UE_LOG(LogTemp, Display, TEXT("========================================"));
	UE_LOG(LogTemp, Display, TEXT("STEP 1: Validate Player"));
	UE_LOG(LogTemp, Display, TEXT("  Player = %s"), Player ? *Player->GetName() : TEXT("NULL"));
	
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("  ERROR: Player is NULL!"));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("  ✓ Player valid"));
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("STEP 2: Check for Completed Quests"));
	
	TArray<UQuestDefinition*> CompletedQuests = GetCompletedQuests(Player);
	UE_LOG(LogTemp, Display, TEXT("  Found %d completed quest(s)"), CompletedQuests.Num());
	
	if (CompletedQuests.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("  ✓ Player has completed quests to turn in"));
		UE_LOG(LogTemp, Display, TEXT(""));
		UE_LOG(LogTemp, Display, TEXT("STEP 3: Turn In Quest"));
		
		UQuestDefinition* QuestToTurnIn = CompletedQuests[0];
		UE_LOG(LogTemp, Display, TEXT("  Quest to turn in: %s"), *QuestToTurnIn->QuestID.ToString());
		
		bool bSuccess = TurnInQuest(QuestToTurnIn->QuestID, Player);
		
		if (bSuccess)
		{
			UE_LOG(LogTemp, Display, TEXT("  ✓ Quest turned in successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("  ✗ Failed to turn in quest!"));
		}
		
		UE_LOG(LogTemp, Display, TEXT(""));
		UE_LOG(LogTemp, Display, TEXT("========================================"));
		UE_LOG(LogTemp, Display, TEXT("QUEST FLOW: HandleInteraction END (Turn-in)"));
		UE_LOG(LogTemp, Display, TEXT("========================================"));
		UE_LOG(LogTemp, Display, TEXT(""));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("  No completed quests found"));
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("STEP 3: Offer New Quest"));
	
	TArray<UQuestDefinition*> QuestsToOffer = GetAvailableQuests(Player);
	UE_LOG(LogTemp, Display, TEXT("  Available quests: %d"), QuestsToOffer.Num());
	
	if (QuestsToOffer.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("  Quest to offer: %s"), *QuestsToOffer[0]->QuestID.ToString());
		OfferQuest(QuestsToOffer[0], Player);
		UE_LOG(LogTemp, Display, TEXT("  ✓ Quest offered"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("  ✗ No quests available to offer"));
	}
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("========================================"));
	UE_LOG(LogTemp, Display, TEXT("QUEST FLOW: HandleInteraction END (Offer)"));
	UE_LOG(LogTemp, Display, TEXT("========================================"));
	UE_LOG(LogTemp, Display, TEXT(""));
}

TArray<UQuestDefinition*> UQuestGiverComponent::GetCompletedQuests(AActor* Player)
{
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: GetCompletedQuests called for player %s"), Player ? *Player->GetName() : TEXT("NULL"));
	
	TArray<UQuestDefinition*> CompletedQuests;
	
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST GIVER: Player is NULL!"));
		return CompletedQuests;
	}
	
	// Get QuestSubsystem
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST GIVER: GameInstance is NULL!"));
		return CompletedQuests;
	}
	
	UQuestSubSystem* QuestSub = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSub)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> QUEST GIVER: QuestSubsystem is NULL!"));
		return CompletedQuests;
	}
	
	// Check each quest this NPC offers to see if player has completed it
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Checking %d available quest(s) for completion"), AvailableQuests.Num());
	
	for (UQuestDefinition* Quest : AvailableQuests)
	{
		if (!Quest)
		{
			continue;
		}
		
		// Check if this quest is active and completed in the subsystem
		FActiveQuest* ActiveQuest = QuestSub->FindActiveQuest(Quest->QuestID);
		if (ActiveQuest && ActiveQuest->State == EQuestState::Completed)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Found completed quest: %s (State=%d)"), 
				*Quest->QuestID.ToString(), (int32)ActiveQuest->State);
			CompletedQuests.Add(Quest);
		}
		else if (ActiveQuest)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Quest %s is active but not completed (State=%d)"), 
				*Quest->QuestID.ToString(), (int32)ActiveQuest->State);
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Found %d completed quest(s) to turn in"), CompletedQuests.Num());
	
	return CompletedQuests;
}

bool UQuestGiverComponent::TurnInQuest(FName QuestID, AActor* Player)
{
	UE_LOG(LogTemp, Display, TEXT("  ----------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("  TURN-IN FLOW: TurnInQuest START"));
	UE_LOG(LogTemp, Display, TEXT("  ----------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("  STEP 1: Validate Input"));
	UE_LOG(LogTemp, Display, TEXT("    QuestID = %s"), *QuestID.ToString());
	UE_LOG(LogTemp, Display, TEXT("    Player = %s"), Player ? *Player->GetName() : TEXT("NULL"));
	
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("    ERROR: Player is NULL!"));
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("    Input valid"));
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  STEP 2: Get QuestSubsystem"));
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("    ERROR: GameInstance is NULL!"));
		return false;
	}
	
	UQuestSubSystem* QuestSub = GameInstance->GetSubsystem<UQuestSubSystem>();
	if (!QuestSub)
	{
		UE_LOG(LogTemp, Error, TEXT("    ERROR: QuestSubsystem is NULL!"));
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("    QuestSubsystem found"));
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  STEP 3: Find Active Quest"));
	
	FActiveQuest* ActiveQuest = QuestSub->FindActiveQuest(QuestID);
	if (!ActiveQuest)
	{
		UE_LOG(LogTemp, Warning, TEXT("    Quest not found in active quests!"));
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("    Quest found in active quests"));
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  STEP 4: Verify Quest State"));
	UE_LOG(LogTemp, Display, TEXT("    Current state = %d (0=NotStarted, 1=Active, 2=Completed, 3=Failed)"), (int32)ActiveQuest->State);
	
	if (ActiveQuest->State != EQuestState::Completed)
	{
		UE_LOG(LogTemp, Warning, TEXT("    Quest is not completed yet!"));
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("    Quest is completed"));
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  STEP 5: Apply Rewards"));
	UE_LOG(LogTemp, Display, TEXT("    XP: %d"), ActiveQuest->QuestDefinition->Rewards.ExperiencePoints);
	UE_LOG(LogTemp, Display, TEXT("    Gold: %d"), ActiveQuest->QuestDefinition->Rewards.Gold);
	
	QuestSub->ApplyQuestRewards(Player, ActiveQuest->QuestDefinition->Rewards);
	UE_LOG(LogTemp, Display, TEXT("    Rewards applied"));
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  STEP 6: Remove Quest from Active List"));
	
	bool bRemoved = QuestSub->RemoveQuest(QuestID);
	if (bRemoved)
	{
		UE_LOG(LogTemp, Display, TEXT("    Quest removed from active quests"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("    Failed to remove quest!"));
	}
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  STEP 7: Broadcast Turn-in Event"));
	
	// Broadcast with quest definition and rewards so UI can show completion dialog
	OnQuestTurnedIn.Broadcast(QuestID, ActiveQuest->QuestDefinition, Player);
	UE_LOG(LogTemp, Display, TEXT("    Event broadcasted (Quest=%s, Player=%s)"), 
		*QuestID.ToString(), *Player->GetName());
	
	UE_LOG(LogTemp, Display, TEXT(""));
	UE_LOG(LogTemp, Display, TEXT("  ----------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("  TURN-IN FLOW: TurnInQuest SUCCESS"));
	UE_LOG(LogTemp, Display, TEXT("  ----------------------------------------"));
	return true;
}
