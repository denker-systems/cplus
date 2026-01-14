// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerProgressionComponent.h"

UPlayerProgressionComponent::UPlayerProgressionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerProgressionComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UPlayerProgressionComponent::AddExperience(int32 Amount)
{
	if (Amount <= 0 || CurrentLevel >= MaxLevel)
	{
		return false;
	}

	CurrentExperience += Amount;
	OnExperienceGained.Broadcast(Amount, CurrentExperience);

	CheckLevelUp();

	return CurrentLevel < MaxLevel;
}

int32 UPlayerProgressionComponent::GetExperienceRequiredForNextLevel() const
{
	if (CurrentLevel >= MaxLevel)
	{
		return 0;
	}

	return CalculateExperienceForLevel(CurrentLevel + 1);
}

float UPlayerProgressionComponent::GetLevelProgress() const
{
	if (CurrentLevel >= MaxLevel)
	{
		return 1.0f;
	}

	int32 CurrentLevelXP = CalculateExperienceForLevel(CurrentLevel);
	int32 NextLevelXP = CalculateExperienceForLevel(CurrentLevel + 1);
	int32 XPIntoLevel = CurrentExperience - CurrentLevelXP;
	int32 XPRequiredForLevel = NextLevelXP - CurrentLevelXP;

	if (XPRequiredForLevel <= 0)
	{
		return 1.0f;
	}

	return FMath::Clamp(static_cast<float>(XPIntoLevel) / static_cast<float>(XPRequiredForLevel), 0.0f, 1.0f);
}

void UPlayerProgressionComponent::AddGold(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	Gold += Amount;
	OnGoldChanged.Broadcast(Amount, Gold);
}

bool UPlayerProgressionComponent::RemoveGold(int32 Amount)
{
	if (Amount <= 0 || Gold < Amount)
	{
		return false;
	}

	Gold -= Amount;
	OnGoldChanged.Broadcast(-Amount, Gold);
	return true;
}

bool UPlayerProgressionComponent::HasGold(int32 Amount) const
{
	return Gold >= Amount;
}

void UPlayerProgressionComponent::AddReputation(FName FactionID, int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}

	int32* CurrentRep = FactionReputation.Find(FactionID);
	if (CurrentRep)
	{
		*CurrentRep += Amount;
	}
	else
	{
		FactionReputation.Add(FactionID, Amount);
	}
}

int32 UPlayerProgressionComponent::GetReputation(FName FactionID) const
{
	const int32* Rep = FactionReputation.Find(FactionID);
	return Rep ? *Rep : 0;
}

void UPlayerProgressionComponent::CheckLevelUp()
{
	int32 OldLevel = CurrentLevel;
	
	while (CurrentLevel < MaxLevel)
	{
		int32 XPRequired = CalculateExperienceForLevel(CurrentLevel + 1);
		if (CurrentExperience >= XPRequired)
		{
			CurrentLevel++;
		}
		else
		{
			break;
		}
	}

	if (CurrentLevel > OldLevel)
	{
		OnLevelUp.Broadcast(CurrentLevel, OldLevel);
	}
}

int32 UPlayerProgressionComponent::CalculateExperienceForLevel(int32 Level) const
{
	if (Level <= 1)
	{
		return 0;
	}

	int32 TotalXP = 0;
	for (int32 i = 2; i <= Level; ++i)
	{
		TotalXP += FMath::RoundToInt(BaseExperienceRequired * FMath::Pow(ExperienceMultiplier, i - 2));
	}

	return TotalXP;
}
