# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- GameDataSubsystem for centralized DataAsset management with UAssetManager
- Complete character hierarchy: BaseCharacter → BasePlayerCharacter/BaseAICharacter → specialized classes
- Quest dialog system (FQuestDialogData with speaker, text, audio, portrait)
- Quest cutscene system (FQuestCutsceneData with LevelSequence integration)
- Quest mission/stage system (FQuestMissionData, FQuestStageData)
- Quest failure conditions (FQuestFailureCondition with time limits, NPC death, etc.)
- Quest actors: AQuestGiverNPC, AQuestMarker, ADialogActor, AQuestTriggerVolume
- BasePlayerCharacter with QuestTracker, Inventory, Interaction components
- BaseAICharacter with Health, Team system, QuestTarget component
- AFriendlyAICharacter with QuestGiver component and IQuestInteractable
- AHostileAICharacter with IQuestKillable interface
- Organized Source folder structure (Core, QuestSystem, InventorySystem, InteractionSystem)

### Changed
- QuestDefinition expanded with stages, missions, dialogs, cutscenes, audio, failure conditions
- cplusCharacter refactored to inherit from BasePlayerCharacter
- ShooterNPC refactored to inherit from HostileAICharacter
- All quest files organized into Data, Runtime, Components, Tasks, Interfaces, UI, Actors subfolders
- cplus.Build.cs updated with editor modules (UnrealEd, Blutility, UMGEditor, AssetRegistry) and LevelSequence
- Updated include paths for new folder structure

### Fixed
- FInventoryItem::InstanceID initialization error with UPROPERTY Meta tag
- GameplayTags not appearing (added to DefaultGameplayTags.ini with ImportTagsFromConfig)
- ShooterNPC Die() signature to match BaseAICharacter (added Killer parameter)
- ShooterNPC aiming logic for AI (uses actor eye location instead of FirstPersonCamera)

### Removed
- MyTestActor, MyTestClass (test files)
- Data Registry approach (reverted to DataAssets + AssetManager)
- QuestRegistryRow, QuestRegistrySubsystem, QuestEditorWidget (abandoned Data Registry implementation)

---

## [0.1.0] - 2026-01-13

### Added
- Initial Unreal Engine 5.7 project setup
- Quest System architecture
- Core Character classes
- Project structure for multiple game variants
- QuestSubsystem implementation
- QuestTrackerComponent
- QuestGiverComponent
- QuestTargetComponent
- Quest interaction interfaces
- Component-based architecture
- GameplayTags integration
- Delegate-based event system

### Changed
- N/A

### Fixed
- UHT errors with struct pointers in UFUNCTION
- Memory management with UPROPERTY references
- Delegate binding issues

### Removed
- N/A

---

## [Future]

### Planned features
- [ ] Quest reward system
- [ ] Quest UI implementation
- [ ] Save/Load system
- [ ] Shooter variant implementation
- [ ] Horror variant implementation
- [ ] Performance optimization
- [ ] Unit tests

---

**Version:** 0.1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
