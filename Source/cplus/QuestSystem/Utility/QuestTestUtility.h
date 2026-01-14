#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestTestUtility.generated.h"

class UQuestDefinition;

/**
 * Quest Test Utility
 * 
 * Utility class for generating test quest definitions programmatically.
 * Use console command: GenerateTestQuests
 */
UCLASS()
class CPLUS_API UQuestTestUtility : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Generate test quest definitions
	 * Creates multiple test quests in /Game/Plugin/Quests/TestQuests/
	 * 
	 * @param NumQuests Number of test quests to generate (default: 3)
	 */
	UFUNCTION(Exec)
	static void GenerateTestQuests(int32 NumQuests = 3);

	/**
	 * Create Quest Generator Widget Blueprint
	 * Creates an Editor Utility Widget Blueprint from QuestGeneratorWidget C++ class
	 */
	UFUNCTION(Exec)
	static void CreateQuestGeneratorWidget();

private:
	/**
	 * Create a single test quest
	 * 
	 * @param QuestIndex Index for unique naming
	 * @return Created quest definition
	 */
	static UQuestDefinition* CreateTestQuest(int32 QuestIndex);
};
