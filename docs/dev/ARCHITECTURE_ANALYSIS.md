# Architecture Analysis Report

**Date:** 2026-01-14  
**Analyst:** Cascade AI  
**Project:** cplus (UE 5.7)

---

## Executive Summary

The codebase has **significant architectural issues** that create confusion, maintenance burden, and potential bugs:

1. **Parallel class hierarchies** that don't connect properly
2. **Duplicated functionality** across multiple classes
3. **Legacy wrapper classes** that add no value
4. **Variant classes bypassing base classes**

**Estimated technical debt:** ~30% of Core module code is redundant or improperly structured.

---

## 1. Critical Issues

### 1.1 Broken Class Hierarchies

#### PlayerController Hierarchy
```
EXPECTED:
APlayerController
    └── ABasePlayerController
            ├── AShooterPlayerController
            └── AHorrorPlayerController

ACTUAL:
APlayerController
    ├── ABasePlayerController (UNUSED - nothing inherits from it!)
    ├── AcplusPlayerController (near-duplicate of ABasePlayerController)
    ├── AShooterPlayerController (inherits APlayerController directly!)
    └── AHorrorPlayerController (inherits APlayerController directly!)
```

**Problem:** `ShooterPlayerController` and `HorrorPlayerController` inherit directly from `APlayerController`, completely bypassing both `BasePlayerController` and `cplusPlayerController`. This means:
- Input setup is duplicated in each variant
- Mobile controls code is copy-pasted 3 times
- No shared base functionality

#### GameMode Hierarchy
```
EXPECTED:
AGameModeBase
    └── ABaseGameMode
            ├── AShooterGameMode
            └── AHorrorGameMode

ACTUAL:
AGameModeBase
    ├── ABaseGameMode (UNUSED - nothing inherits from it!)
    ├── AcplusGameMode (empty shell, inherits AGameModeBase)
    ├── AShooterGameMode (inherits AGameModeBase directly!)
    └── AHorrorGameMode (inherits AGameModeBase directly!)
```

**Problem:** Same issue - variant GameModes bypass `BaseGameMode`, losing Main Menu functionality.

### 1.2 Legacy Wrapper Classes (Should Be Removed)

| Class | Purpose | Status |
|-------|---------|--------|
| `AcplusCharacter` | "Legacy class - all functionality now in BasePlayerCharacter" | **DELETE** |
| `AcplusGameMode` | Empty shell, not inherited by anything useful | **DELETE** |
| `AcplusPlayerController` | Near-duplicate of ABasePlayerController | **CONSOLIDATE** |
| `AFriendlyAICharacter` | Only sets Team=Friendly, 21 lines | **CONSIDER REMOVING** |
| `AHostileAICharacter` | Only sets Team=Hostile, 21 lines | **CONSIDER REMOVING** |

### 1.3 Duplicate Interactable Systems

Two parallel systems exist:

**InteractionSystem:**
- `AInteractableObject` - Base for doors, chests, etc.
- `UInteractionComponent` - Component for interaction
- `UInteractableDefinition` - DataAsset for definitions

**QuestSystem/Actors:**
- `AQuestInteractableObject` - Does the same thing!
- Has its own sphere component, prompt text, etc.

**Problem:** Both implement `IQuestInteractable` but don't share code.

---

## 2. Duplicated Code Analysis

### 2.1 Input/Mobile Controls (Copy-Pasted 4 Times!)

The following code block appears in:
- `ABasePlayerController`
- `AcplusPlayerController`
- `AShooterPlayerController`
- `AHorrorPlayerController`

```cpp
UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
TArray<UInputMappingContext*> DefaultMappingContexts;

UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
TArray<UInputMappingContext*> MobileExcludedMappingContexts;

UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
TSubclassOf<UUserWidget> MobileControlsWidgetClass;

UPROPERTY()
TObjectPtr<UUserWidget> MobileControlsWidget;

UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
bool bForceTouchControls = false;

bool ShouldUseTouchControls() const;
```

### 2.2 Similar UI Manager Classes

- `UUIManager` - Central UI manager (PauseMenu, references QuestUIManager)
- `UQuestUIManager` - Quest-specific UI manager

Both are `UActorComponent` with similar patterns. Could potentially be consolidated.

---

## 3. Unused/Dead Files

### 3.1 Potentially Unused (No External Includes)

These files are only included by their own .cpp files:

| File | Reason | Recommendation |
|------|--------|----------------|
| `cplusCameraManager.h` | Only used by cplusPlayerController and HorrorPlayerController | Keep if camera features needed |
| `cplus.h` | Module header, only basic macros | Keep |
| `QuestMarker.h` | Only self-referenced | Verify Blueprint usage |
| `DialogActor.h` | Only self-referenced | Verify Blueprint usage |

### 3.2 Near-Empty Classes

| File | Lines | Content |
|------|-------|---------|
| `FriendlyAICharacter.cpp` | 206 bytes | Just constructor setting Team |
| `HostileAICharacter.cpp` | 200 bytes | Just constructor setting Team |
| `HorrorGameMode.cpp` | 154 bytes | Just constructor |
| `cplusCharacter.cpp` | 177 bytes | Just constructor |

---

## 4. QuestSystem Complexity

The QuestSystem has **71 files** which is significant. Structure analysis:

```
QuestSystem/
├── Actors/      (12 files) - Quest-related actors
├── Components/  (6 files)  - Quest components ✓ Good
├── Data/        (15 files) - DataAssets and structs
├── Interfaces/  (6 files)  - Quest interfaces ✓ Good
├── Runtime/     (2 files)  - QuestSubSystem
├── Tasks/       (12 files) - Task types ✓ Good
├── UI/          (14 files) - UI widgets
└── Utility/     (4 files)  - Test/Generator tools
```

**Observations:**
- Data/ has 15 files for structs/DataAssets - could consolidate
- UI/ has 14 files - appropriate for complexity
- Tasks/ well-organized with base + variants

---

## 5. Recommended Actions

### Priority 1: Fix Class Hierarchies (CRITICAL)

1. **Make ShooterPlayerController inherit from BasePlayerController**
   ```cpp
   // CHANGE FROM:
   class AShooterPlayerController : public APlayerController
   // CHANGE TO:
   class AShooterPlayerController : public ABasePlayerController
   ```

2. **Make HorrorPlayerController inherit from BasePlayerController**
   
3. **Make ShooterGameMode inherit from BaseGameMode**
   
4. **Make HorrorGameMode inherit from BaseGameMode**

5. **Remove cplusPlayerController** (consolidate into BasePlayerController)

6. **Remove cplusGameMode** (no purpose)

7. **Remove cplusCharacter** (legacy wrapper)

### Priority 2: Consolidate Duplicate Code

1. Remove duplicate input/mobile code from variant controllers
2. Consolidate InteractableObject and QuestInteractableObject

### Priority 3: Consider Removing Thin Wrappers

1. `FriendlyAICharacter` → Use `ABaseAICharacter` with Team=Friendly
2. `HostileAICharacter` → Use `ABaseAICharacter` with Team=Hostile
3. Or keep for Blueprint organization (valid reason)

---

## 6. Proposed Clean Architecture

```
CHARACTERS:
ACharacter
└── ABaseCharacter (tags, common functionality)
    ├── ABasePlayerCharacter (player input, components)
    │   ├── AShooterCharacter (weapons)
    │   └── AHorrorCharacter (stamina/sprint)
    └── ABaseAICharacter (AI, health, quest integration)
        └── AShooterNPC (shooter-specific AI)

CONTROLLERS:
APlayerController
└── ABasePlayerController (input contexts, mobile)
    ├── AShooterPlayerController (respawn, bullet UI)
    └── AHorrorPlayerController (horror UI)

GAME MODES:
AGameModeBase
└── ABaseGameMode (main menu, pause)
    ├── AShooterGameMode (scores, shooter UI)
    └── AHorrorGameMode (horror-specific)

INTERACTABLES:
AActor
└── AInteractableObject (IQuestInteractable)
    └── AQuestInteractableObject (quest-specific features)
```

---

## 7. File Count Summary

| Module | Files | Status |
|--------|-------|--------|
| Core | 32 | 🟡 Has redundancy |
| QuestSystem | 71 | 🟢 Well-structured |
| Variant_Shooter | 28 | 🟡 Hierarchy issues |
| Variant_Horror | 8 | 🟡 Hierarchy issues |
| InteractionSystem | 6 | 🟡 Overlaps with Quest |
| InventorySystem | 10 | 🟢 Good |
| SaveSystem | 3 | 🟢 Good |
| UI | 8 | 🟢 Good |

**Total: ~166 source files**

---

## Next Steps

1. Review this analysis
2. Decide which refactoring to prioritize
3. Create backup branch before major changes
4. Implement fixes incrementally with testing

---

*Report generated by architecture analysis session*
