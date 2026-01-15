# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added (2026-01-14)
- **WeaponSystem folder structure** - Organized weapon code into Actors/Components/Data
- **BasePlayerAnimInstance** - C++ animation instance with Speed, Direction, bIsInAir, weapon states
- **ReloadAction input** - R key for weapon reload
- **SpringArm + FollowCamera** - Third-person camera setup
- **AnimBP switching system** - SetAnimInstanceClass() in OnWeaponActivated/Deactivated
- **Core Redirects** - ShooterCharacter→BasePlayerCharacter, weapon class redirects

### Added
- **Save/Load System** - Complete save game system with 3 save slots
  - **SaveGameData** - Data structures for quest/player/inventory/health saves
  - **SaveGameSubsystem** - Save/load manager with slot management
  - **NewGame()** - Start new game in specified slot
  - **SaveGame()** - Save current state to slot
  - **LoadGame()** - Load game from slot
  - **Continue()** - Load most recent save automatically
  - **QuickSave()** - Save to current active slot
  - **Save slot metadata** - Player name, save time, level, playtime
  - **LoadQuestProgress()** - Restore quest state from save data
- **Main Menu System** - Complete main menu with overlay integration
  - **MainMenuWidget** - Main menu with New Game/Continue/Load/Settings/Exit
  - **MainMenuActor** - Placeable actor with camera for Main Menu scene
  - **BaseGameMode integration** - Main Menu as optional overlay
  - **bShowMainMenuOnStart** - Toggle Main Menu for development/production
  - **Auto-find MainMenuActor** - Automatically finds or spawns Main Menu actor
  - **Camera switching** - Smooth blend between Main Menu and gameplay cameras
- **Pause Menu Enhancements** - Added Save/Load/Main Menu navigation
  - **Save Game button** - Quick save to current slot
  - **Load Game button** - Load from current slot
  - **Main Menu button** - Return to Main Menu from pause
- **Quest UI State Management** - Hide Quest UI when Main Menu active
  - **QuestUIManager::HideUI()** - Hide all quest widgets
  - **QuestUIManager::ShowUI()** - Restore quest widgets
  - **bIsMainMenuActive flag** - Track Main Menu state for UI
- **Health Component Save/Load** - SetHealth() for restoring saved health
- **Quest Completion Widget** - Dedicated UI for quest turn-in rewards
- **FActiveQuest.TaskProgress** - Array for quest progress (prepared for refactor)
- **Quest Progress Visualization System** - Complete progress tracking for all quest types
  - **QuestProgressWidget** - Displays detailed quest progress with objectives and tasks
  - **QuestTaskEntryWidget** - Individual task display with progress bars and counters
  - **GetProgressText()** - Formatted progress text for tasks (e.g., "3/10", "50%")
  - **GetTaskTypeName()** - Task type identification (Collect, Kill, Interact, Explore, Talk)
  - **GetDetailedProgressText()** - Full task description with progress (e.g., "Collect: 3/10 Gold Coins")
  - **Task type icons** - Visual indicators for each quest task type
  - **Color-coded progress** - Green (complete), Yellow (in progress), Gray (not started)
  - **Real-time progress updates** - Progress bars update as tasks complete
- **QuestTask_Talk** - New task type for NPC conversations with GameplayTag and ID matching
- **QuestUIManager** - Component for managing quest notification and journal widgets
- **QuestNotificationWidget** - Shows quest started/completed/failed notifications
- **QuestJournalWidget** - Quest journal UI with J key toggle (IA_OpenJournal input action)
- **QuestCollectible** - Actor for collectible quest items with IQuestInteractable interface
- **QuestInteractableObject** - Actor for interactable quest objects
- **PlayerProgressionComponent** - Manages player XP, gold, reputation, and level progression
- **TextRenderComponent prompts** - "Press E to talk/collect" prompts on interactables
- **ShowPrompt/HidePrompt** - Added to IQuestInteractable interface for interaction prompts
- **ApplyQuestRewards** - Quest reward application in QuestSubsystem
- **HealthComponent** - Component-based health management for players and AI
- **WeaponComponent** - Component-based weapon inventory and switching system
- **BaseGameMode** - Clean game mode using BasePlayerCharacter and BasePlayerController
- **BasePlayerController** - Controller with Enhanced Input Mapping Context support (IMC_Default + IMC_MouseLook)
- Debug logging system for GameMode, PlayerController, and PlayerCharacter initialization
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
- Quest giver interaction flow with quest offer UI (Accept/Decline)
- Quest generator tools (Editor Utility Widget + console utility) for creating test QuestDefinition assets
- Auto-loading quest definitions for quest givers from a content folder
- Organized Source folder structure (Core, QuestSystem, InventorySystem, InteractionSystem)

### Fixed
- **Quest Turn-In System** - Fixed GetCompletedQuests() to correctly identify completed quests
  - Changed from GetActiveQuests() filter to direct FindActiveQuest() state check
  - Completed quests now properly detected for turn-in after game reset
  - Quest completion widget now displays correctly with rewards
- **Duplicate Quest Acceptance** - Added check to prevent accepting same quest multiple times
  - Validates quest is not already active or completed before accepting
  - Prevents duplicate quest instances in ActiveQuests map
- **Quest UI Visibility on Main Menu** - Quest Tracker no longer shows on Main Menu
  - Added bIsMainMenuActive flag for state tracking
  - QuestUIManager checks Main Menu state when creating widgets
  - Widgets auto-hide if Main Menu is active

### Changed
- **BasePlayerCharacter** - Consolidated ALL player features: FPS components, shooter features, horror features (sprint/stamina), Enhanced Input
- **BaseAICharacter** - Consolidated ALL AI features: shooter AI (aiming, shooting, ragdoll), quest interfaces, weapon handling
- **FriendlyAICharacter** - Simplified to only set Team = Friendly (all functionality in base)
- **HostileAICharacter** - Simplified to only set Team = Hostile (all functionality in base)
- **AcplusCharacter** - Simplified to legacy wrapper (all functionality moved to BasePlayerCharacter)
- **AShooterCharacter** - Simplified, removed duplicate members now in BasePlayerCharacter
- **AHorrorCharacter** - Simplified, removed duplicate members now in BasePlayerCharacter
- **AShooterNPC** - Simplified, removed duplicate members now in BaseAICharacter
- QuestDefinition expanded with stages, missions, dialogs, cutscenes, audio, failure conditions
- All quest files organized into Data, Runtime, Components, Tasks, Interfaces, UI, Actors subfolders
- cplus.Build.cs updated with editor modules (UnrealEd, Blutility, UMGEditor, AssetRegistry) and LevelSequence
- Updated include paths for new folder structure

### Fixed
- **NotifyQuestEvent** - Now checks all active quests when QuestID is NAME_None
- **QuestTrackerComponent::AcceptQuest** - Now properly notifies QuestSubsystem
- **NPC interaction prompts** - TextRenderComponent with billboard effect (faces camera)
- **Character hierarchy shadowing errors** - Removed duplicate UPROPERTY members in child classes
- **Input system** - Configured both IMC_Default (keyboard/gamepad) and IMC_MouseLook (mouse) for proper input handling
- **Camera attachment** - Fixed FirstPersonCamera attachment to FirstPersonMesh head socket
- **Component initialization** - All components now properly created in base classes
- FInventoryItem::InstanceID initialization error with UPROPERTY Meta tag
- GameplayTags not appearing (added to DefaultGameplayTags.ini with ImportTagsFromConfig)
- ShooterNPC Die() signature to match BaseAICharacter (added Killer parameter)
- ShooterNPC aiming logic for AI (uses actor eye location instead of FirstPersonCamera)
- Quest giver interaction edge case where interact could trigger while the quest UI was open

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
