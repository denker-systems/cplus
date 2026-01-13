# Code Commentary Standards

> **Destination:** `.windsurf/rules/code-commentary.md`

---
trigger: always_on
description: Code commentary standards for C++ source files
---

# Code Commentary Standards

> Guidelines for clear, concise English comments in C++ code

## 🎯 Purpose

Ensure code is understandable for team members through:
- Clear class and function documentation
- Explanation of complex logic
- Notes on integration points
- Warnings about edge cases

**Balance:** Just enough for clarity - not too much, not too little.

---

## Commentary Rules

### Language
```xml
<commentary_language>
- ALL code comments MUST be in English
- Use clear, professional technical English
- Avoid slang or informal language
- Be concise and direct
</commentary_language>
```

### What to Comment
```xml
<must_comment>
- Class purpose and usage (all UCLASS)
- Public API functions (all UFUNCTION)
- Complex algorithms and logic
- Non-obvious implementation details
- Integration points with other systems
- Workarounds and hacks (with explanation)
- Performance-critical sections
- Thread safety considerations
- Magic numbers (replace with named constants)
- TODOs and FIXMEs
</must_comment>
```

### What NOT to Comment
```xml
<dont_comment>
- Obvious code (i++, return true, etc.)
- Self-explanatory function names
- Standard Unreal Engine patterns
- Every single line of code
- Redundant information
- Code that speaks for itself
</dont_comment>
```

---

## Comment Styles

### Class-Level (Doxygen Style)

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
};
```

### Function Documentation

```cpp
/**
 * Accept a new quest and add it to active quests
 * 
 * Validates the quest, checks if already active, then adds to
 * the active quests list and notifies the QuestSubsystem.
 * 
 * @param Quest The quest definition to accept (must not be null)
 * @return True if quest was accepted, false if already active or null
 */
UFUNCTION(BlueprintCallable, Category = "Quest")
bool AcceptQuest(UQuestDefinition* Quest);
```

### Inline Comments

```cpp
// Single line for simple explanations
void UpdateHealth()
{
    // Clamp health between 0 and max
    Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}
```

### Block Comments

```cpp
/*
 * Complex quest completion logic:
 * 1. Verify all objectives complete
 * 2. Calculate rewards based on performance
 * 3. Apply rewards to player
 * 4. Broadcast completion event
 * 5. Remove from active quests
 */
void CompleteQuest(FName QuestID)
{
    // Implementation
}
```

---

## Specific Guidelines

### UPROPERTY Comments

```cpp
// Brief description is enough for properties
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
FName QuestID;  // Unique identifier for this quest

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
int32 CurrentCount;  // Current progress count for quest objective

UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
TArray<TObjectPtr<UQuestTask>> Tasks;  // All tasks that must be completed
```

### Delegate Comments

```cpp
/**
 * Broadcast when a quest is accepted
 * 
 * @param QuestID The ID of the accepted quest
 * @param Quest The quest definition
 */
UPROPERTY(BlueprintAssignable, Category = "Quest")
FOnQuestAccepted OnQuestAccepted;
```

### Interface Comments

```cpp
/**
 * Quest Interactable Interface
 * 
 * Implement this interface on actors that can be interacted with
 * for quest purposes (NPCs, objects, etc.)
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UQuestInteractable : public UInterface
{
    GENERATED_BODY()
};

class CPLUS_API IQuestInteractable
{
    GENERATED_BODY()

public:
    /**
     * Called when player interacts with this object for a quest
     * 
     * @param Interactor The actor initiating the interaction (usually player)
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
    void Interact(AActor* Interactor);
};
```

---

## Special Cases

### TODOs and FIXMEs

```cpp
// TODO: Implement quest reward calculation
void CalculateRewards()
{
    // Placeholder
}

// FIXME: This crashes if Quest is null - add null check
void ProcessQuest(UQuestDefinition* Quest)
{
    Quest->QuestID;  // Potential crash
}

// HACK: Workaround for UE 5.7 bug with subsystem initialization
// Remove this when Epic fixes the bug in 5.8
void InitializeSubsystem()
{
    // Workaround code
}
```

### Performance Notes

```cpp
/**
 * Update all active quests
 * 
 * Performance: O(n*m) where n = active quests, m = tasks per quest
 * Called every frame - keep this optimized!
 */
void UpdateAllQuests()
{
    // Cache subsystem to avoid repeated GetSubsystem calls
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    
    for (auto& Pair : ActiveQuests)
    {
        // Update logic
    }
}
```

### Thread Safety

```cpp
/**
 * Thread-safe quest completion
 * 
 * WARNING: Can be called from any thread
 * Uses mutex to protect ActiveQuests map
 */
void CompleteQuestThreadSafe(FName QuestID)
{
    FScopeLock Lock(&QuestMutex);
    // Implementation
}
```

---

## Commentary Density Guidelines

### Target Ratios:
- **Header files (.h):** 20-30% comment lines
- **Implementation files (.cpp):** 10-20% comment lines
- **Complex algorithms:** 30-40% comment lines
- **Simple getters/setters:** 0-5% comment lines

### Check Commentary Density:
```powershell
function Get-CommentDensity {
    param($FilePath)
    
    $content = Get-Content $FilePath
    $totalLines = $content.Count
    $commentLines = ($content | Select-String "//|/\*|\*/" | Measure-Object).Count
    $ratio = if ($totalLines -gt 0) { [math]::Round(($commentLines / $totalLines) * 100, 1) } else { 0 }
    
    Write-Host "$FilePath: $ratio% comments"
}

# Check all files
Get-ChildItem -Recurse -Include "*.cpp","*.h" -Path "Source/cplus" | ForEach-Object {
    Get-CommentDensity $_.FullName
}
```

---

## Best Practices

### DO ✅
- Write comments as you code
- Update comments when code changes
- Use Doxygen style for public API
- Explain WHY, not just WHAT
- Document integration points
- Mark TODOs clearly
- Use @see tags for related classes
- Keep comments concise

### DON'T ❌
- Comment obvious code
- Write redundant comments
- Use comments to disable code (delete it instead)
- Write comments in Swedish
- Over-comment simple code
- Leave outdated comments
- Use comments instead of good naming

---

## Examples by File Type

### Component Header
```cpp
/**
 * Quest Giver Component
 * 
 * Manages available quests for an NPC and handles quest offering to players.
 * When player interacts, presents list of available quests.
 * 
 * @see UQuestTrackerComponent
 * @see IQuestInteractable
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestGiverComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** List of quests this NPC can offer */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<TObjectPtr<UQuestDefinition>> AvailableQuests;
    
    /**
     * Offer quests to the player
     * @param Player The player to offer quests to
     */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void OfferQuest(AActor* Player);
};
```

### Subsystem Header
```cpp
/**
 * Quest Subsystem
 * 
 * Global manager for all quest-related functionality.
 * Tracks active quests, manages quest state, and broadcasts quest events.
 * 
 * Access via: GetGameInstance()->GetSubsystem<UQuestSubsystem>()
 * 
 * @see UQuestDefinition
 * @see UQuestTrackerComponent
 */
UCLASS()
class CPLUS_API UQuestSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    /**
     * Accept a new quest and add to active quests
     * 
     * @param Quest The quest to accept
     * @return True if accepted, false if already active
     */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool AcceptQuest(UQuestDefinition* Quest);

private:
    /** Map of all currently active quests, keyed by QuestID */
    UPROPERTY()
    TMap<FName, FActiveQuest> ActiveQuests;
};
```

### Interface Header
```cpp
/**
 * Quest Killable Interface
 * 
 * Implement on actors that can be killed for quest objectives.
 * Notifies quest system when killed.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UQuestKillable : public UInterface
{
    GENERATED_BODY()
};

class CPLUS_API IQuestKillable
{
    GENERATED_BODY()

public:
    /**
     * Called when this actor is killed for a quest
     * @param Killer The actor that killed this (usually player)
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
    void OnKilledForQuest(AActor* Killer);
};
```

---

## Quality Checklist

### Before Committing:
- [ ] All new classes have documentation
- [ ] All new public functions documented
- [ ] Complex logic explained
- [ ] No Swedish comments
- [ ] No redundant comments
- [ ] No commented-out code
- [ ] TODOs marked if incomplete
- [ ] @see tags for related classes

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
