// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiverComponent.h"
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

	UE_LOG(LogTemp, Display, TEXT(">>> QUEST GIVER: Auto-loaded %d new quest(s), total: %d"), 
		LoadedCount, AvailableQuests.Num());
}

TArray<UQuestDefinition*> UQuestGiverComponent::GetAvailableQuests(AActor* Player)
{
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Called on instance %p"), this);
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Owner = %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: AvailableQuests.Num() = %d"), AvailableQuests.Num());
	
	TArray<UQuestDefinition*> Result;
	for (int32 i = 0; i < AvailableQuests.Num(); i++)
	{
		UQuestDefinition* Quest = AvailableQuests[i];
		UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Quest[%d] = %p"), i, Quest);
		
		if (Quest)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> GET AVAILABLE QUESTS: Adding quest: %s"), *Quest->QuestID.ToString());
			Result.Add(Quest);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> GET AVAILABLE QUESTS: Quest[%d] is NULL!"), i);
		}
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
