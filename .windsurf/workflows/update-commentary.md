# Unreal Engine Code Commentary Update Workflow

> **Destination:** `.windsurf/workflows/update-commentary.md`

---
description: Review and update code comments in source files for team clarity
auto_execution_mode: 1
---

# Update Commentary Workflow

> Systematic review and improvement of code comments in C++ source files

## 🎯 Mission

Ensure all source files have clear, concise English comments that help team members understand:
- What the code does
- Why it does it
- How it integrates with other systems
- Important edge cases or gotchas

**Balance:** Not too much, not too little - just enough for clarity.

---

## Step 1: Find All Source Files

```powershell
# List all C++ files
Get-ChildItem -Recurse -Include "*.h","*.cpp" -Path "Source/cplus" | Select-Object FullName

# Count files
$files = Get-ChildItem -Recurse -Include "*.h","*.cpp" -Path "Source/cplus"
Write-Host "Total files to review: $($files.Count)" -ForegroundColor Cyan
```

---

## Step 2: Review Files by System

### Quest System Files
```powershell
# Quest System
Get-ChildItem -Recurse -Include "*.h","*.cpp" -Path "Source/cplus/QuestSystem" | Select-Object Name
```

### Shooter Variant Files
```powershell
# Shooter Variant
Get-ChildItem -Recurse -Include "*.h","*.cpp" -Path "Source/cplus/Variant_Shooter" | Select-Object Name
```

### Horror Variant Files
```powershell
# Horror Variant
Get-ChildItem -Recurse -Include "*.h","*.cpp" -Path "Source/cplus/Variant_Horror" | Select-Object Name
```

---

## Step 3: Check Each File

### For Each File, Check:

1. **Class-level comment** - Does it explain what the class does?
2. **Public methods** - Are they documented?
3. **Complex logic** - Is it explained?
4. **Magic numbers** - Are they explained?
5. **Workarounds** - Are they documented?
6. **TODOs** - Are they marked clearly?

### Quick Check Script
```powershell
# Find files with no class comments
Get-ChildItem -Recurse -Include "*.h" -Path "Source/cplus" | ForEach-Object {
    $content = Get-Content $_.FullName -Raw
    if ($content -notmatch "/\*\*[\s\S]*?\*/\s*UCLASS") {
        Write-Host "Missing class comment: $($_.Name)" -ForegroundColor Yellow
    }
}
```

---

## Step 4: Commentary Standards

### Class-Level Comments (Header Files)

```cpp
/**
 * Quest Tracker Component
 * 
 * Tracks the player's active quests and broadcasts quest-related events.
 * Integrates with QuestSubsystem for centralized quest management.
 * 
 * Usage: Attach to player character to enable quest tracking.
 * 
 * @see UQuestSubsystem
 * @see UQuestDefinition
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestTrackerComponent : public UActorComponent
{
    GENERATED_BODY()
    // ...
};
```

### Public Method Comments

```cpp
/**
 * Accept a new quest and add it to the active quests list
 * 
 * @param Quest The quest definition to accept (must not be null)
 * @return True if quest was successfully accepted, false otherwise
 */
UFUNCTION(BlueprintCallable, Category = "Quest")
bool AcceptQuest(UQuestDefinition* Quest);
```

### Complex Logic Comments

```cpp
void UQuestSubsystem::UpdateQuestProgress(FName QuestID)
{
    // Find the active quest
    FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
    if (!ActiveQuest)
    {
        return;
    }
    
    // Check all objectives in current objective index
    // We only check current objective, not all objectives at once
    // This ensures linear quest progression
    const FQuestObjective& CurrentObj = ActiveQuest->Definition->Objectives[ActiveQuest->CurrentObjectiveIndex];
    
    // Evaluate all tasks in this objective
    bool bAllTasksComplete = true;
    for (const TObjectPtr<UQuestTask>& Task : CurrentObj.Tasks)
    {
        if (!Task->IsComplete())
        {
            bAllTasksComplete = false;
            break;
        }
    }
    
    // If all tasks complete, move to next objective
    if (bAllTasksComplete)
    {
        ActiveQuest->CurrentObjectiveIndex++;
        // ... rest of logic
    }
}
```

### Inline Comments (When Needed)

```cpp
void UQuestGiverComponent::OfferQuest(AActor* Player)
{
    // Validate player
    if (!Player)
    {
        return;
    }
    
    // Get quest tracker from player
    UQuestTrackerComponent* Tracker = Player->FindComponentByClass<UQuestTrackerComponent>();
    if (!Tracker)
    {
        UE_LOG(LogQuest, Warning, TEXT("Player has no QuestTrackerComponent"));
        return;
    }
    
    // Filter quests player can accept
    // Skip quests that are already active or completed
    TArray<UQuestDefinition*> AvailableForPlayer;
    for (UQuestDefinition* Quest : AvailableQuests)
    {
        if (Quest && !Tracker->HasQuest(Quest->QuestID))
        {
            AvailableForPlayer.Add(Quest);
        }
    }
    
    // Show UI with available quests
    // TODO: Implement quest selection UI
}
```

### Magic Numbers and Constants

```cpp
// ❌ BAD - No explanation
if (Health < 20.0f)
{
    Die();
}

// ✅ GOOD - Explained
const float kCriticalHealthThreshold = 20.0f;  // Below this, NPC dies instantly
if (Health < kCriticalHealthThreshold)
{
    Die();
}
```

---

## Step 5: What NOT to Comment

### Don't Comment Obvious Code

```cpp
// ❌ BAD - Obvious
// Set health to 100
Health = 100.0f;

// ❌ BAD - Redundant
// Call BeginPlay
BeginPlay();

// ✅ GOOD - Adds value
// Reset health to maximum for new game
Health = kMaxHealth;
```

### Don't Over-Comment

```cpp
// ❌ BAD - Too much
// This function accepts a quest
// It takes a UQuestDefinition pointer as parameter
// It returns a boolean value
// True means success, false means failure
bool AcceptQuest(UQuestDefinition* Quest);

// ✅ GOOD - Concise
/**
 * Accept a new quest
 * @param Quest Quest to accept (must not be null)
 * @return True if accepted successfully
 */
bool AcceptQuest(UQuestDefinition* Quest);
```

---

## Step 6: Update Files Systematically

### Process:
1. Open file
2. Read class-level comment - is it clear?
3. Read each public method - are they documented?
4. Read complex logic - is it explained?
5. Add/update comments as needed
6. Move to next file

### Priority Order:
1. **High Priority:** Public API (headers)
2. **Medium Priority:** Complex implementation logic
3. **Low Priority:** Simple getters/setters

---

## Step 7: Commentary Checklist

### For Each Header File (.h):
- [ ] Class has documentation comment
- [ ] Class purpose is clear
- [ ] Integration points mentioned (@see tags)
- [ ] All public UFUNCTION documented
- [ ] All public UPROPERTY have brief description
- [ ] Complex nested structures explained

### For Each Implementation File (.cpp):
- [ ] Complex algorithms explained
- [ ] Non-obvious logic commented
- [ ] Magic numbers have constants with comments
- [ ] Workarounds documented with reason
- [ ] TODOs marked clearly
- [ ] No redundant/obvious comments

---

## Step 8: Review Examples

### Example 1: Quest Tracker Component

**Before:**
```cpp
// QuestTrackerComponent.h
UCLASS()
class CPLUS_API UQuestTrackerComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable)
    void AcceptQuest(UQuestDefinition* Quest);
};
```

**After:**
```cpp
// QuestTrackerComponent.h
/**
 * Quest Tracker Component
 * 
 * Tracks the player's active quests and manages quest progression.
 * Broadcasts events when quests are accepted, updated, or completed.
 * 
 * Attach this component to the player character to enable quest tracking.
 * 
 * @see UQuestSubsystem For global quest management
 * @see UQuestDefinition For quest data structure
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestTrackerComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    /**
     * Accept a new quest and add it to active quests
     * 
     * @param Quest The quest to accept (must not be null)
     * @return True if quest was accepted, false if already active or null
     */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool AcceptQuest(UQuestDefinition* Quest);
};
```

### Example 2: Quest Subsystem

**Before:**
```cpp
// QuestSubsystem.cpp
void UQuestSubsystem::UpdateQuestProgress(FName QuestID)
{
    FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
    if (!ActiveQuest)
    {
        return;
    }
    
    const FQuestObjective& CurrentObj = ActiveQuest->Definition->Objectives[ActiveQuest->CurrentObjectiveIndex];
    
    bool bAllTasksComplete = true;
    for (const TObjectPtr<UQuestTask>& Task : CurrentObj.Tasks)
    {
        if (!Task->IsComplete())
        {
            bAllTasksComplete = false;
            break;
        }
    }
    
    if (bAllTasksComplete)
    {
        ActiveQuest->CurrentObjectiveIndex++;
    }
}
```

**After:**
```cpp
// QuestSubsystem.cpp
void UQuestSubsystem::UpdateQuestProgress(FName QuestID)
{
    // Find the quest in active quests
    FActiveQuest* ActiveQuest = ActiveQuests.Find(QuestID);
    if (!ActiveQuest)
    {
        UE_LOG(LogQuest, Warning, TEXT("Quest not found: %s"), *QuestID.ToString());
        return;
    }
    
    // Get current objective (quests progress linearly through objectives)
    const FQuestObjective& CurrentObj = ActiveQuest->Definition->Objectives[ActiveQuest->CurrentObjectiveIndex];
    
    // Check if all tasks in current objective are complete
    bool bAllTasksComplete = true;
    for (const TObjectPtr<UQuestTask>& Task : CurrentObj.Tasks)
    {
        if (!Task->IsComplete())
        {
            bAllTasksComplete = false;
            break;
        }
    }
    
    // Move to next objective if current is complete
    if (bAllTasksComplete)
    {
        ActiveQuest->CurrentObjectiveIndex++;
        UE_LOG(LogQuest, Log, TEXT("Objective complete, moving to next"));
        
        // Check if quest is fully complete
        if (ActiveQuest->CurrentObjectiveIndex >= ActiveQuest->Definition->Objectives.Num())
        {
            CompleteQuest(QuestID);
        }
    }
}
```

---

## Step 9: Find Files Needing Commentary

```powershell
# Find files with low comment density
Get-ChildItem -Recurse -Include "*.cpp","*.h" -Path "Source/cplus" | ForEach-Object {
    $content = Get-Content $_.FullName
    $totalLines = $content.Count
    $commentLines = ($content | Select-String "//|/\*|\*/" | Measure-Object).Count
    $ratio = if ($totalLines -gt 0) { [math]::Round(($commentLines / $totalLines) * 100, 1) } else { 0 }
    
    if ($ratio -lt 10) {  # Less than 10% comments
        Write-Host "$($_.Name): $ratio% comments" -ForegroundColor Yellow
    }
}
```

---

## Step 10: Quick Reference

### When to Comment:

**DO Comment:**
- ✅ Class purpose and usage
- ✅ Public API functions
- ✅ Complex algorithms
- ✅ Non-obvious logic
- ✅ Workarounds and hacks
- ✅ Integration points
- ✅ Performance considerations
- ✅ Thread safety notes

**DON'T Comment:**
- ❌ Obvious code (i++ increments i)
- ❌ Self-explanatory function names
- ❌ Standard patterns
- ❌ Every single line

### Comment Style:

**Class/Function (Doxygen style):**
```cpp
/**
 * Brief description
 * 
 * Detailed explanation if needed
 * 
 * @param ParamName Description
 * @return Description
 * @see RelatedClass
 */
```

**Inline (C++ style):**
```cpp
// Single line explanation
```

**Block (for complex sections):**
```cpp
/*
 * Multi-line explanation
 * of complex logic
 */
```

---

## Workflow Checklist

- [ ] All header files reviewed
- [ ] All implementation files reviewed
- [ ] Class-level comments added/updated
- [ ] Public methods documented
- [ ] Complex logic explained
- [ ] Magic numbers replaced with constants
- [ ] TODOs marked clearly
- [ ] No redundant comments
- [ ] All comments in English
- [ ] Build still works after changes

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
