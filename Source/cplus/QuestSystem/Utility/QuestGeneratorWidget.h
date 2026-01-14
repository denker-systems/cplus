#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuestGeneratorWidget.generated.h"

class UQuestDefinition;

/**
 * Quest Generator Widget
 * 
 * Editor Utility Widget for generating test quest definitions with UI.
 * Provides a user-friendly interface for creating multiple test quests.
 */
UCLASS()
class CPLUS_API UQuestGeneratorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	/** Number of quests to generate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Generation", meta = (ClampMin = 1, ClampMax = 20))
	int32 NumQuests = 3;

	/** Prefix for quest names */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Generation")
	FString QuestPrefix = TEXT("TestQuest");

	/** Output folder path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Generation")
	FString OutputFolder = TEXT("/Game/Plugin/Quests/TestQuests");

	/** Generate test quests */
	UFUNCTION(BlueprintCallable, Category = "Quest Generation")
	void GenerateQuests();

	/** Clear all generated quests */
	UFUNCTION(BlueprintCallable, Category = "Quest Generation")
	void ClearGeneratedQuests();

	/** Get list of generated quest names */
	UFUNCTION(BlueprintPure, Category = "Quest Generation")
	TArray<FString> GetGeneratedQuestNames() const { return GeneratedQuestNames; }

private:
	/** Create a single test quest */
	UQuestDefinition* CreateTestQuest(int32 QuestIndex);

	/** List of generated quest names */
	UPROPERTY()
	TArray<FString> GeneratedQuestNames;
};
