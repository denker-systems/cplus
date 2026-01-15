// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class cplus : ModuleRules
{
	public cplus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"GameplayTags",
			"UMG",
			"Slate",
			"LevelSequence"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",
				"Blutility",
				"UMGEditor",
				"AssetRegistry"
			});
		}

		PublicIncludePaths.AddRange(new string[] {
			"cplus",
			"cplus/Core",
			"cplus/QuestSystem",
			"cplus/QuestSystem/Data",
			"cplus/QuestSystem/Runtime",
			"cplus/QuestSystem/Tasks",
			"cplus/QuestSystem/Interfaces",
			"cplus/QuestSystem/Components",
			"cplus/QuestSystem/UI",
			"cplus/QuestSystem/Actors",
			"cplus/InventorySystem",
			"cplus/InteractionSystem",
			"cplus/UI",
			"cplus/WeaponSystem",
			"cplus/WeaponSystem/Actors",
			"cplus/WeaponSystem/Components",
			"cplus/WeaponSystem/Data",
			"cplus/CombatSystem",
			"cplus/CombatSystem/Core",
			"cplus/CombatSystem/Animation",
			"cplus/CombatSystem/Interfaces",
			"cplus/AISystem",
			"cplus/SaveSystem"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
