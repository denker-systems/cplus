# Known Issues - cplus Project

**Last Updated:** 2026-01-14  
**Project:** cplus (Unreal Engine 5.7)

---

## 🚨 CRITICAL - Quest Progress Modifies DataAsset

**Severity:** CRITICAL  
**Status:** IDENTIFIED - Needs Refactoring  
**Discovered:** 2026-01-14

### Problem Description

Quest task progress (`Task->CurrentCount`) is stored directly in `UQuestDefinition` DataAssets, which are `UPrimaryDataAsset` singletons. This causes quest progress to persist between New Game sessions.

### Technical Details

**Current Implementation:**
```cpp
// ❌ WRONG - Modifies DataAsset at runtime
void UQuestSubSystem::LoadQuestProgress(...)
{
    Task->CurrentCount = TaskProgress[i];  // Modifies singleton!
}

void UQuestSubSystem::UpdateTaskProgress(...)
{
    Task->UpdateProgress(1);  // Modifies singleton!
}

void UQuestSubSystem::ResetAllQuests()
{
    Task->CurrentCount = 0;  // Modifies singleton!
}
```

**Why This Is Wrong:**
- `UQuestDefinition` is `UPrimaryDataAsset` (loaded once, reused everywhere)
- Same DataAsset instance shared across all quest instances
- Modifications persist in memory between PIE sessions
- New Game doesn't truly reset quest progress
- Multiple players would share same quest progress

### Impact

1. **New Game Broken** - Quest progress from previous game carries over
2. **Save/Load Unreliable** - Loading old save affects DataAsset state
3. **Multiplayer Impossible** - All players would share quest progress
4. **Memory Leak** - DataAsset modified state never cleaned up

### Solution

**Refactor to use `FActiveQuest.TaskProgress` array:**

```cpp
// ✅ CORRECT - Store progress in instance struct
struct FActiveQuest
{
    TObjectPtr<UQuestDefinition> QuestDefinition;  // Immutable reference
    EQuestState State;
    int32 CurrentObjectiveIndex;
    TArray<int32> TaskProgress;  // Runtime progress stored HERE
    FDateTime AcceptedTime;
};

// Read progress from instance, not DataAsset
int32 GetTaskProgress(FActiveQuest& Quest, int32 TaskIndex)
{
    return Quest.TaskProgress[TaskIndex];  // ✅ CORRECT
}

// Write progress to instance, not DataAsset
void UpdateTaskProgress(FActiveQuest& Quest, int32 TaskIndex, int32 Amount)
{
    Quest.TaskProgress[TaskIndex] += Amount;  // ✅ CORRECT
}
```

### Files to Refactor

**High Priority:**
1. `QuestSubSystem.cpp` - AcceptQuest, UpdateTaskProgress, ProcessQuestEvent, ResetAllQuests, LoadQuestProgress
2. `questNotificationWidget.cpp` - PopulateTrackerContent, GetTaskProgressColor
3. `QuestUIManager.cpp` - ShowObjectiveUpdated
4. `QuestProgressWidget.cpp` - GetTaskProgressColor
5. `QuestTaskEntryWidget.cpp` - UpdateTaskProgress

**Medium Priority:**
6. All quest task classes - Remove CurrentCount usage
7. Quest generator tools - Update to not set CurrentCount

### Refactoring Steps

1. **Initialize TaskProgress in AcceptQuest()**
   ```cpp
   void AcceptQuest(UQuestDefinition* Quest)
   {
       FActiveQuest NewQuest;
       NewQuest.QuestDefinition = Quest;
       
       // Initialize TaskProgress array
       if (Quest->Objectives.Num() > 0)
       {
           for (UQuestTask* Task : Quest->Objectives[0].Tasks)
           {
               NewQuest.TaskProgress.Add(0);  // Start at 0
           }
       }
       
       ActiveQuests.Add(Quest->QuestID, NewQuest);
   }
   ```

2. **Update ProcessQuestEvent() to modify TaskProgress**
   ```cpp
   void ProcessQuestEvent(...)
   {
       // OLD: Task->UpdateProgress(1);
       // NEW:
       ActiveQuest.TaskProgress[TaskIndex] += 1;
   }
   ```

3. **Update UI to read from FActiveQuest.TaskProgress**
   ```cpp
   // OLD: Task->CurrentCount
   // NEW: ActiveQuest.TaskProgress[TaskIndex]
   ```

4. **Remove CurrentCount from QuestTask entirely**
   ```cpp
   // Remove this member variable:
   // int32 CurrentCount = 0;
   ```

### Testing Plan

After refactoring:
1. Start New Game → Accept quest → Complete task → Progress = 1
2. Start New Game again → Accept same quest → Progress should be 0 (not 1)
3. Save Game → Load Game → Progress should match saved state
4. Multiple quests with same definition → Each has independent progress

### Estimated Effort

**Time:** 1-2 hours  
**Risk:** Medium (touches many files)  
**Priority:** CRITICAL (blocks proper Save/Load functionality)

---

## ⚠️ MINOR - Camera Doesn't Switch on Pause → Main Menu

**Severity:** MINOR  
**Status:** PARTIALLY FIXED  
**Discovered:** 2026-01-14

### Problem Description

When navigating from Pause Menu → Main Menu, the camera doesn't always switch to MainMenuActor camera view.

### Technical Details

**Current Behavior:**
```
Gameplay (Player Camera)
    ↓ ESC
Pause Menu (Player Camera)
    ↓ Main Menu button
Main Menu (Player Camera)  ← Should be MainMenuActor camera
```

**Expected Behavior:**
```
Main Menu (MainMenuActor Camera) ✅
```

### Root Cause

MainMenuActor stores `OriginalViewTarget` on first activation. When going Pause → Main Menu:
1. Current view target is already player character
2. OriginalViewTarget was already set to player
3. SetViewTargetWithBlend() called but no visual change

### Current Workaround

```cpp
// Check if current view target is MainMenuActor before storing
if (CurrentViewTarget != this)
{
    OriginalViewTarget = CurrentViewTarget;
}
```

### Status

Partially fixed - needs testing to verify camera switches correctly.

### Priority

LOW - Cosmetic issue, doesn't affect functionality

---

## 📋 Other Known Issues

### Issue: InputMode Focus Warning

**Severity:** COSMETIC  
**Message:** `InputMode:UIOnly - Attempting to focus Non-Focusable widget SObjectWidget`

**Impact:** None (warning only, doesn't affect functionality)

**Cause:** Trying to focus widget that doesn't have Focusable flag set

**Solution:** Set widget as Focusable in Blueprint or ignore warning

---

## 🔄 Deferred Features

### 1. Save Slot Selection UI

**Status:** NOT IMPLEMENTED  
**Current:** New Game/Load always use slot 0  
**Needed:** UI to select which slot to save/load

**Implementation Plan:**
- Create SaveLoadMenuWidget
- Show all 3 slots with metadata
- Allow clicking slot to select
- Show "Empty Slot" for unused slots

### 2. Settings Menu

**Status:** PLACEHOLDER  
**Current:** Settings button shows warning  
**Needed:** Graphics, audio, controls settings

### 3. Inventory Save/Load

**Status:** PARTIALLY IMPLEMENTED  
**Current:** Saves ItemID and Quantity, restores via GameDataSubsystem  
**Issue:** Requires GameDataSubsystem::GetItemByID() to be implemented  
**Workaround:** Currently logs warning if item not found

---

## 📝 Notes for Next Session

### Before Starting Next Session:

1. **Commit current work** - Save/Load system implementation
2. **Review session report** - docs/dev/sessions/2026-01-14_save-load-system.md
3. **Read KNOWN_ISSUES.md** - This file
4. **Plan refactoring** - Quest progress to FActiveQuest.TaskProgress

### Next Session Priorities:

1. **CRITICAL:** Refactor quest progress storage (2 hours)
2. **HIGH:** Test Save/Load functionality end-to-end
3. **MEDIUM:** Remove Reset button after verification
4. **LOW:** Fix camera switch issue

---

**Document Version:** 1.0  
**Maintained By:** Development Team  
**Review Frequency:** After each session
