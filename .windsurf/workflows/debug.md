# Unreal Engine Debug Workflow

> **Destination:** `.windsurf/workflows/debug.md`

---
description: Debugging Unreal Engine 5.7 C++ project
auto_execution_mode: 1
---

# Debug Workflow

> Systematic debugging of Unreal Engine 5.7 C++ project

## 1. Identify Problem Type

### Problem Categories
- 🔴 **Build Errors** - Compilation errors
- 🟠 **Linker Errors** - Linking errors  
- 🟡 **Runtime Crashes** - Crashes during execution
- 🟢 **Logic Errors** - Incorrect behavior
- 🔵 **Performance** - Performance issues
- 🟣 **Memory Leaks** - Memory leaks
- 🟤 **Rendering Issues** - Graphics problems

---

## 2. Build Errors

### Common Compiler Errors

| Error | Cause | Solution |
|-------|-------|---------|
| `error C2065: undeclared identifier` | Variable/function not declared | Add #include or declaration |
| `error C2039: is not a member` | Wrong method/variable | Check class and name |
| `error C2664: cannot convert` | Wrong type in parameter | Cast to correct type or change parameter |
| `error C2248: cannot access` | Private/protected member | Change access or use public method |
| `error C2512: no appropriate default constructor` | Class lacks default constructor | Add constructor or initialize correctly |

### Unreal Header Tool (UHT) Errors

| Error | Cause | Solution |
|-------|-------|---------|
| `Inappropriate '*' on variable` | Pointer to struct in UFUNCTION | Remove UFUNCTION or use const ref |
| `Unknown type` | Type not known to UHT | Include header before .generated.h |
| `GENERATED_BODY() missing` | UCLASS without macro | Add GENERATED_BODY() |
| `Multiple definitions` | Multiple definitions | Use #pragma once |

### Debug Build Errors
```powershell
# Find all errors
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" 2>&1 | Select-String "error"

# Find specific file
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" 2>&1 | Select-String "QuestSubsystem"

# Verbose output
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -verbose
```

---

## 3. Runtime Crashes

### Find Crash Logs
```powershell
# Unreal Editor crash logs
Get-ChildItem "Saved\Logs" -Filter "*.log" | Sort-Object LastWriteTime -Descending | Select-Object -First 5

# Windows Event Viewer
eventvwr.msc
# Windows Logs → Application → Filter for "UnrealEditor"
```

### Common Crash Causes

| Symptom | Cause | Solution |
|---------|-------|---------|
| Access Violation | Null pointer dereference | Add null checks |
| Stack Overflow | Infinite recursion | Add base case or limit depth |
| Assertion Failed | check() failed | Fix condition causing assertion |
| Pure Virtual Call | Call to pure virtual in constructor | Don't call virtual functions in constructors |

### Debug with Visual Studio

1. **Start with debugger**
   - Open cplus.sln
   - Set cplusEditor as Startup Project
   - Press F5 (Start Debugging)

2. **Breakpoints**
   - Click in left margin to set breakpoint
   - F9 to toggle breakpoint
   - Shift+F9 for conditional breakpoint

3. **Call Stack**
   - Debug → Windows → Call Stack (Ctrl+Alt+C)
   - See execution chain at crash

4. **Watch Window**
   - Debug → Windows → Watch (Ctrl+Alt+W, 1)
   - Monitor variable values

---

## 4. Unreal Engine Debugging Tools

### Output Log
```powershell
# Open Output Log in Editor
# Window → Developer Tools → Output Log

# Log categories
LogQuest      # Quest system logs
LogAI         # AI system logs  
LogTemp       # Temporary logs
LogInit       # Initialization logs
```

### Console Commands
```
# Log verbosity
[Category] LogLevel Verbose
LogQuest LogLevel All

# Show FPS
stat fps

# Show performance
stat unit

# Show collision
show collision

# Show navigation
show navigation
```

### Visual Logger
```
# Activate with ' key
# Shows AI navigation, perception, etc.
```

### Blueprint Debugger
```
# Alt+D to activate
# Set breakpoints in Blueprint graphs
```

---

## 5. Quest System Debugging

### Common Quest Problems

| Problem | Symptom | Debug Steps |
|---------|---------|------------|
| Quest not accepted | Interact does nothing | 1. Check QuestGiverComponent<br>2. Verify AvailableQuests<br>3. Check delegate binding |
| Progress not updating | Task count stuck | 1. Check NotifyQuest<br>2. Verify GameplayTags<br>3. Check UpdateTaskProgress |
| Quest complete not triggered | Quest not marked as complete | 1. Check IsComplete logic<br>2. Verify Objective completion<br>3. Check CompleteQuest call |

### Debug Code for Quest
```cpp
// Add to QuestSubsystem
void UQuestSubsystem::DebugPrintActiveQuests()
{
    UE_LOG(LogQuest, Log, TEXT("=== ACTIVE QUESTS ==="));
    for (auto& Pair : ActiveQuests)
    {
        FName QuestID = Pair.Key;
        const FActiveQuest& Quest = Pair.Value;
        UE_LOG(LogQuest, Log, TEXT("Quest: %s, State: %d, Objectives: %d"), 
            *QuestID.ToString(), (int)Quest.State, Quest.CurrentObjectiveIndex);
    }
}

// Call from console
UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
if (QuestSub)
{
    QuestSub->DebugPrintActiveQuests();
}
```

---

## 6. Memory Debugging

### Memory Leaks
```cpp
// Use TObjectPtr for UPROPERTY
UPROPERTY()
TObjectPtr<UQuestDefinition> Quest;  // ✅ Garbage collected

// NOT raw pointer without UPROPERTY
UQuestDefinition* Quest;  // ❌ Will be garbage collected!

// For temporary pointers
TWeakObjectPtr<AActor> WeakActor;  // ✅ Weak reference
```

### Memory Profiler
```
# Start with -memleak flag
UnrealEditor.exe cplus.uproject -memleak

# Use Unreal Insights
# Window → Developer Tools → Insights
```

---

## 7. Performance Debugging

### Stat Commands
```
stat fps              # FPS counter
stat unit             # Frame time breakdown
stat game             # Game thread time
stat render           # Render thread time
stat gpu              # GPU time
stat memory           # Memory usage
stat particles        # Particle count
stat streaming        # Asset streaming
```

### Common Performance Issues

| Issue | Symptom | Fix |
|-------|---------|-----|
| Tick every frame | FPS drop | Use events instead of Tick |
| Many casts | Frame spikes | Cache cast results |
| Large arrays | Memory usage | Use TArray with reserve |
| String operations | CPU usage | Use FName for constant strings |

### Profile Code
```cpp
// Use SCOPE_CYCLE_COUNTER
SCOPE_CYCLE_COUNTER(STAT_QuestUpdate);

// Or manual timing
double StartTime = FPlatformTime::Seconds();
// ... code ...
double EndTime = FPlatformTime::Seconds();
UE_LOG(LogQuest, Log, TEXT("Quest update took: %f seconds"), EndTime - StartTime);
```

---

## 8. AI Debugging

### AI Debug Tools
```
show debug            # Show AI debug info
show collision        # Show collision
show navigation       # Show navigation mesh
LogAI LogLevel All    # All AI logs
```

### Common AI Problems
- **NPC stuck** - Check navigation mesh
- **No path** - Verify NavSystem
- **Behavior tree not running** - Check AIController

---

## 9. Common Debug Scenarios

### Scenario 1: Quest System Not Working
```powershell
# 1. Check build status
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# 2. Check Output Log for quest errors
Get-Content "Saved\Logs\UnrealEditor.log" | Select-String "LogQuest|Error|Warning"

# 3. Verify components in Blueprint
# Open Character Blueprint → Check that QuestTrackerComponent exists
```

### Scenario 2: Editor Crashes on Start
```powershell
# 1. Check crash log
Get-Content "Saved\Logs\CrashReportClient.log" | Select-Object -Last 50

# 2. Remove corrupted files
Remove-Item "Saved\Config\WindowsEditor\EditorPerProjectUserSettings.ini" -Force

# 3. Restart
```

### Scenario 3: Live Coding Fails
```powershell
# 1. Check what changed
git diff --name-only

# 2. If .h files changed → Restart editor
# 3. If new classes → Restart editor
# 4. If only .cpp → Try again
```

---

## 10. Debug Checklist

### Build Issues
- [ ] All headers included correctly
- [ ] GENERATED_BODY() in all UCLASS
- [ ] UPROPERTY/UFUNCTION declared correctly
- [ ] .Build.cs updated with dependencies
- [ ] No syntax errors in IDE

### Runtime Issues
- [ ] Null checks for all pointers
- [ ] Components initialized in BeginPlay
- [ ] Events bound correctly
- [ ] GameplayTags defined
- [ ] No infinite recursion

### Performance
- [ ] Tick only if necessary
- [ ] Casts cached
- [ ] String operations minimized
- [ ] TArray with reserve
- [ ] Stat profiler used

---

## Quick Reference

### Debug Commands
```powershell
# Build with verbose output
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -verbose

# Find errors in log
Get-Content "Saved\Logs\UnrealEditor.log" | Select-String "Error"

# Start with debugger
# Open cplus.sln → F5

# Console commands (in editor)
stat fps
show collision
LogQuest LogLevel All
```

### Common Fixes
```cpp
// Null pointer crash
if (QuestSubsystem)
{
    QuestSubsystem->AcceptQuest(Quest);
}

// UHT error with struct pointer
UFUNCTION(BlueprintCallable)
void UpdateQuest(const FQuestData& QuestData);  // ✅ Const ref

// NOT
void UpdateQuest(FQuestData* QuestData);  // ❌ Pointer to struct
```

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
