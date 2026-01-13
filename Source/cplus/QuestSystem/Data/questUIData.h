// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Texture2D.h"
#include "questUIData.generated.h"

/**
 * Quest UI Data
 * 
 * Separates UI presentation from quest logic.
 * Makes localization and UI customization easier.
 */
USTRUCT(BlueprintType)
struct CPLUS_API FQuestUIData
{
	GENERATED_BODY()

	/** Quest title shown in UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText QuestTitle;

	/** Short description for quest list (1-2 sentences) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (MultiLine = true))
	FText ShortDescription;

	/** Full quest description for journal (detailed backstory) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (MultiLine = true))
	FText FullDescription;

	/** Icon for quest in UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> QuestIcon;

	/** Quest category for filtering (e.g., Quest.Type.Main, Quest.Type.Side) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FGameplayTag QuestCategory;

	/** Color tint for quest in UI (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FLinearColor QuestColor = FLinearColor::White;
};
