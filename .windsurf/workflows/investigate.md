# Unreal Engine Deep Investigation Workflow

> **Destination:** `.windsurf/workflows/investigate.md`

---
description: Systematic problem-solving through data flow analysis and testing
auto_execution_mode: 1
---

# Investigate Workflow

> Deep-dive investigation to find and fix problems systematically

## 🎯 Mission

Find root cause and implement solution through:
1. Trace complete data flow
2. Read all related files
3. Test with debug logging
4. Isolate the problem
5. Fix and verify

---

## Step 1: Understand the Problem

**Ask:**
- What's broken? (specific symptom)
- Can you reproduce it? (steps)
- What changed recently? (git log)
- Any error messages? (logs)

**Check Context:**
```powershell
# Recent changes
git log --oneline -10
git diff HEAD~1

# Error logs
Get-Content "Saved\Logs\*.log" | Select-String "Error|Warning" | Select-Object -Last 20
```

---

## Step 2: Find ALL Related Files

**Use grep to find everything:**
```powershell
# Find all files with class name
Get-ChildItem -Recurse -Include "*.h","*.cpp" | Select-String "ClassName" | Select-Object Path -Unique

# Find all function calls
Get-ChildItem -Recurse -Include "*.cpp","*.h" | Select-String "FunctionName" | Format-Table Path, LineNumber

# Find variable usage
Get-ChildItem -Recurse -Include "*.cpp","*.h" | Select-String "VariableName"
```

**Critical: Read EVERY file found** - don't skip any!

---

## Step 3: Trace Complete Data Flow

**Map execution from start to end:**

```
Entry Point
    ↓
Function 1 → Read this file
    ↓
Function 2 → Read this file
    ↓
Function 3 → Read this file (Problem here?)
    ↓
Function 4 → Read this file
    ↓
End Point
```

**For EACH function in the chain:**
1. **Read the .h file** - Check declarations
2. **Read the .cpp file** - Check implementation
3. **Check inputs** - What data comes in?
4. **Check outputs** - What data goes out?
5. **Check calls** - What does it call next?
6. **Check state** - What does it modify?

**Mark where you think the problem is** - then verify!

---

## Step 4: Add Debug Logging

**Add UE_LOG at EVERY step:**

```cpp
void UQuestTrackerComponent::AcceptQuest(UQuestDefinition* Quest)
{
    UE_LOG(LogQuest, Log, TEXT("=== AcceptQuest START ==="));
    UE_LOG(LogQuest, Log, TEXT("Quest pointer: %p"), Quest);
    
    if (!Quest)
    {
        UE_LOG(LogQuest, Error, TEXT("Quest is NULL!"));
        return;
    }
    
    UE_LOG(LogQuest, Log, TEXT("QuestID: %s"), *Quest->QuestID.ToString());
    UE_LOG(LogQuest, Log, TEXT("ActiveQuests count: %d"), ActiveQuests.Num());
    
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    UE_LOG(LogQuest, Log, TEXT("QuestSubsystem: %p"), QuestSub);
    
    if (!QuestSub)
    {
        UE_LOG(LogQuest, Error, TEXT("QuestSubsystem is NULL!"));
        return;
    }
    
    // Before calling subsystem
    UE_LOG(LogQuest, Log, TEXT("Calling QuestSubsystem->AcceptQuest"));
    QuestSub->AcceptQuest(Quest);
    UE_LOG(LogQuest, Log, TEXT("QuestSubsystem->AcceptQuest returned"));
    
    // Before broadcasting
    UE_LOG(LogQuest, Log, TEXT("Broadcasting OnQuestAccepted"));
    OnQuestAccepted.Broadcast(Quest->QuestID, Quest);
    UE_LOG(LogQuest, Log, TEXT("OnQuestAccepted broadcasted"));
    
    UE_LOG(LogQuest, Log, TEXT("=== AcceptQuest END ==="));
}
```

**Run and watch Output Log:**
```
Window → Developer Tools → Output Log
Filter: LogQuest
Look for: Where does it STOP? That's where the problem is!
```

---

## Step 5: Check Common Issues

### Issue 1: Null Pointers
```cpp
// Check EVERY pointer before use
if (!Quest) { 
    UE_LOG(LogQuest, Error, TEXT("Quest is null")); 
    return; 
}

if (!QuestSubsystem) { 
    UE_LOG(LogQuest, Error, TEXT("QuestSubsystem is null")); 
    return; 
}

if (!Component) { 
    UE_LOG(LogQuest, Error, TEXT("Component is null")); 
    return; 
}
```

### Issue 2: Not Initialized
```cpp
// Check BeginPlay in ALL components
void UQuestTrackerComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Get subsystem - does it exist?
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    if (!QuestSub)
    {
        UE_LOG(LogQuest, Error, TEXT("QuestSubsystem not initialized!"));
    }
}
```

### Issue 3: Delegates Not Bound
```cpp
// Check if delegate is bound
void UQuestTrackerComponent::BeginPlay()
{
    Super::BeginPlay();
    
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    if (QuestSub)
    {
        // Bind delegate
        QuestSub->OnQuestCompleted.AddDynamic(this, &UQuestTrackerComponent::HandleQuestCompleted);
        UE_LOG(LogQuest, Log, TEXT("Delegate bound successfully"));
    }
}

// Check function signature matches
UFUNCTION()
void HandleQuestCompleted(FName QuestID, const FQuestReward& Rewards);
```

### Issue 4: Wrong UPROPERTY
```cpp
// ❌ BAD - Will be garbage collected!
UQuestDefinition* Quest;

// ✅ GOOD - Protected from GC
UPROPERTY()
TObjectPtr<UQuestDefinition> Quest;
```

### Issue 5: Access to Protected/Private
```cpp
// Check if trying to access protected members
// Use public API instead
QuestSubsystem->PublicMethod();  // ✅ GOOD
QuestSubsystem->ProtectedMember;  // ❌ BAD - won't compile
```

---

## Step 6: Test with Minimal Case

**Create simple test:**

```cpp
// Add to any component for testing
void UQuestTrackerComponent::TestMinimal()
{
    UE_LOG(LogQuest, Log, TEXT("=== MINIMAL TEST START ==="));
    
    // Create test quest
    UQuestDefinition* TestQuest = NewObject<UQuestDefinition>(this);
    TestQuest->QuestID = FName("TestQuest");
    TestQuest->Title = FText::FromString("Test");
    
    // Test the problematic function
    AcceptQuest(TestQuest);
    
    // Verify result
    if (ActiveQuests.Contains(TestQuest->QuestID))
    {
        UE_LOG(LogQuest, Log, TEXT("✅ TEST PASSED"));
    }
    else
    {
        UE_LOG(LogQuest, Error, TEXT("❌ TEST FAILED"));
    }
    
    UE_LOG(LogQuest, Log, TEXT("=== MINIMAL TEST END ==="));
}
```

**Call from console or BeginPlay temporarily**

---

## Step 7: Use Visual Studio Debugger

**Set breakpoints at:**
1. Entry point of problematic function
2. Before suspected problem line
3. After suspected problem line
4. In related functions

**Watch variables:**
- All pointers (check if null)
- All counters (check values)
- All arrays/maps (check size)

**Steps:**
1. Open cplus.sln in Visual Studio
2. Set breakpoints (F9)
3. Start debugging (F5)
4. Reproduce problem
5. Step through code (F10/F11)
6. Watch variable values
7. Check call stack (Ctrl+Alt+C)

---

## Step 8: Isolate the Problem

**Binary search approach:**

```
Does problem happen in Function A? YES
    ↓
Does problem happen in first half of Function A? NO
    ↓
Does problem happen in second half of Function A? YES
    ↓
Does problem happen in line 45-50? YES
    ↓
FOUND: Line 47 is the problem!
```

**Comment out code sections:**
```cpp
void ProblematicFunction()
{
    // Part 1
    DoSomething();
    
    // Part 2 - Comment this out
    // DoSomethingElse();
    
    // Part 3
    DoFinalThing();
}
```

Test after each comment-out to isolate the exact line.

---

## Step 9: Identify Root Cause

**5 Whys:**
1. Why does X fail? → Because Y is null
2. Why is Y null? → Because Z didn't initialize it
3. Why didn't Z initialize it? → Because BeginPlay wasn't called
4. Why wasn't BeginPlay called? → Because component wasn't added
5. Why wasn't component added? → Because Blueprint wasn't updated

**Root Cause:** Blueprint needs component added

**Common Root Causes:**
- Null pointer (missing check or initialization)
- Wrong initialization order (BeginPlay timing)
- Delegate not bound (missing AddDynamic)
- Memory corruption (missing UPROPERTY)
- Race condition (async timing)
- Wrong function signature (delegate mismatch)
- Missing dependency (module not in .Build.cs)
- Incorrect cast (wrong type)

---

## Step 10: Implement Fix

**Fix the root cause, not the symptom!**

```cpp
// ❌ BAD - Fixes symptom
if (Quest == nullptr)
{
    Quest = DefaultQuest;  // Hides the real problem
}

// ✅ GOOD - Fixes root cause
void UQuestTrackerComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize properly
    Quest = LoadQuestFromData();
    if (!Quest)
    {
        UE_LOG(LogQuest, Error, TEXT("Failed to load quest!"));
    }
}
```

**Add defensive checks:**
```cpp
void UQuestTrackerComponent::AcceptQuest(UQuestDefinition* Quest)
{
    // Validate input
    if (!Quest)
    {
        UE_LOG(LogQuest, Error, TEXT("Quest is null"));
        return;
    }
    
    // Check subsystem exists
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    if (!QuestSub)
    {
        UE_LOG(LogQuest, Error, TEXT("QuestSubsystem not found"));
        return;
    }
    
    // Check not already active
    if (ActiveQuests.Contains(Quest->QuestID))
    {
        UE_LOG(LogQuest, Warning, TEXT("Quest already active"));
        return;
    }
    
    // Now safe to proceed
    ActiveQuests.Add(Quest->QuestID);
    QuestSub->AcceptQuest(Quest);
}
```

---

## Step 11: Test the Fix

**Test scenarios:**
1. **Normal case** - Does it work now?
2. **Edge cases** - Null input, duplicate, invalid data
3. **Stress test** - Multiple rapid calls
4. **Integration** - Does it work with other systems?

**Build and run:**
```powershell
# Build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Start editor and test
Start-Process "C:\ue5.7\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

**Verify:**
- [ ] Original problem is fixed
- [ ] No new problems introduced
- [ ] All related systems still work
- [ ] Performance is OK (stat fps)
- [ ] No new warnings/errors

---

## Step 12: Clean Up Debug Code

**Remove temporary logging:**
```cpp
// Remove excessive debug logs (keep important ones)
// UE_LOG(LogQuest, Log, TEXT("=== AcceptQuest START ===")); // Remove
UE_LOG(LogQuest, Log, TEXT("Quest accepted: %s"), *Quest->QuestID.ToString()); // Keep
```

**Remove test functions:**
```cpp
// Remove or comment out test functions
// void TestMinimal() { ... }
```

---

## Investigation Checklist

### Phase 1: Understand (5 min)
- [ ] Problem clearly understood
- [ ] Reproduction steps known
- [ ] Recent changes checked
- [ ] Logs reviewed

### Phase 2: Trace (15 min)
- [ ] All related files found
- [ ] Complete data flow mapped
- [ ] Each file in chain read
- [ ] Execution path traced

### Phase 3: Debug (20 min)
- [ ] Debug logging added everywhere
- [ ] Output log monitored
- [ ] Breakpoints set in VS
- [ ] Variables watched

### Phase 4: Isolate (10 min)
- [ ] Problem isolated to specific function/line
- [ ] Root cause identified (not symptom)
- [ ] Common issues checked

### Phase 5: Fix (15 min)
- [ ] Fix implemented
- [ ] Defensive checks added
- [ ] Code follows Unreal standards

### Phase 6: Verify (10 min)
- [ ] Build succeeds
- [ ] Original problem fixed
- [ ] No regressions
- [ ] Edge cases tested

---

## Common Investigation Patterns

### Pattern 1: Null Pointer Crash
```
Symptom: Access violation, crash
Investigation: Add null checks everywhere
Root Cause: Missing null check or initialization
Fix: Add if (!Pointer) { return; }
```

### Pattern 2: Function Not Called
```
Symptom: Nothing happens
Investigation: Add logs to see if function is called
Root Cause: Event not bound or wrong function name
Fix: Bind delegate correctly, check function signature
```

### Pattern 3: Wrong Value
```
Symptom: Incorrect behavior, wrong numbers
Investigation: Log all variable values
Root Cause: Wrong calculation or uninitialized variable
Fix: Correct calculation, initialize properly
```

### Pattern 4: Timing Issue
```
Symptom: Works sometimes, fails other times
Investigation: Check initialization order
Root Cause: Accessing before initialized
Fix: Ensure proper initialization order, add checks
```

### Pattern 5: Memory Corruption
```
Symptom: Random crashes, corrupted data
Investigation: Check UPROPERTY usage
Root Cause: UObject pointer without UPROPERTY
Fix: Add UPROPERTY() or use TObjectPtr<>
```

---

## Quick Debugging Commands

### Unreal Console Commands
```
# In editor console (~)
stat fps              # Show FPS
stat unit             # Frame time
stat memory           # Memory usage
show collision        # Show collision
show navigation       # Show navmesh
LogQuest LogLevel All # Enable all quest logs
```

### PowerShell Quick Checks
```powershell
# Find all TODOs in code
Get-ChildItem -Recurse -Include "*.cpp","*.h" | Select-String "TODO|FIXME|HACK"

# Find all UE_LOG calls
Get-ChildItem -Recurse -Include "*.cpp" | Select-String "UE_LOG"

# Find all null checks
Get-ChildItem -Recurse -Include "*.cpp" | Select-String "if.*!.*\)"
```

---

## Investigation Strategy

### Start Broad, Then Narrow:
1. **Broad:** Which system? (Quest, AI, UI, etc.)
2. **Medium:** Which component? (Tracker, Giver, Subsystem)
3. **Narrow:** Which function? (AcceptQuest, CompleteQuest)
4. **Specific:** Which line? (Line 45, 67, etc.)

### Use Binary Search:
- Problem in first half or second half?
- Problem in this function or next function?
- Problem before this line or after?

### Test Hypotheses:
- "I think it's X" → Test it → Confirmed or Rejected
- "Maybe it's Y" → Test it → Confirmed or Rejected
- Keep testing until you find it!

---

## Tools to Use

### Code Search
- Use Fast Context (@code_search tool)
- Use grep (PowerShell Select-String)
- Use Visual Studio Find All References (Shift+F12)

### Debugging
- Visual Studio Debugger (F5, F9, F10, F11)
- Unreal Output Log (Window → Developer Tools)
- Visual Logger (' key)
- Blueprint Debugger (Alt+Shift+D)

### Testing
- Console commands in editor
- Temporary test functions
- Minimal reproduction cases
- Stat commands for performance

---

## Remember

**DO:**
- ✅ Read ALL related files
- ✅ Trace COMPLETE data flow
- ✅ Add logging EVERYWHERE
- ✅ Test EVERY hypothesis
- ✅ Fix ROOT CAUSE, not symptom
- ✅ Verify fix works

**DON'T:**
- ❌ Skip files (read them all!)
- ❌ Assume you know the problem (test it!)
- ❌ Fix symptoms (find root cause!)
- ❌ Forget to remove debug code
- ❌ Skip testing edge cases

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
