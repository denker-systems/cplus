---
description: Deep-dive problem solving with data flow analysis
auto_execution_mode: 3
---
# Investigate Workflow


**Mission:** Trace data flow → Read all files → Debug log → Isolate → Fix → Verify

## 1. Understand

**Ask:** What's broken? Reproduce? What changed? Errors?

```powershell
git log --oneline -10; git diff HEAD~1
Get-Content "Saved\Logs\*.log" | Select-String "Error|Warning" | Select-Object -Last 20
```

## 2. Find Files

```powershell
Get-ChildItem -Recurse -Include "*.h","*.cpp" | Select-String "ClassName|FunctionName"
```

**Read EVERY file found!**

## 3. Trace Flow

**Map:** Entry → Func1 → Func2 → Func3 → End

**For each:** Read .h/.cpp, check inputs/outputs/calls/state

## 4. Debug Log

**Add UE_LOG everywhere:**

```cpp
void UQuestTrackerComponent::AcceptQuest(UQuestDefinition* Quest)
{
    UE_LOG(LogQuest, Log, TEXT("=== START ==="));
    UE_LOG(LogQuest, Log, TEXT("Quest: %p"), Quest);
    if (!Quest) { UE_LOG(LogQuest, Error, TEXT("NULL!")); return; }
    
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    UE_LOG(LogQuest, Log, TEXT("QuestSub: %p"), QuestSub);
    if (!QuestSub) { UE_LOG(LogQuest, Error, TEXT("NULL!")); return; }
    
    QuestSub->AcceptQuest(Quest);
    UE_LOG(LogQuest, Log, TEXT("=== END ==="));
}
```

**Output Log:** Window → Developer Tools → Output Log (Filter: LogQuest)

## 5. Common Issues

**Null:** `if (!Quest) { UE_LOG(...); return; }`  
**Not Init:** Check BeginPlay, GetSubsystem  
**Delegate:** AddDynamic in BeginPlay  
**UPROPERTY:** Use `UPROPERTY() TObjectPtr<>` not raw pointer

## 6. Test Minimal

```cpp
void TestMinimal() {
    UQuestDefinition* Test = NewObject<UQuestDefinition>(this);
    Test->QuestID = FName("Test");
    AcceptQuest(Test);
    UE_LOG(LogQuest, Log, TEXT("%s"), ActiveQuests.Contains(Test->QuestID) ? TEXT("✅ PASS") : TEXT("❌ FAIL"));
}
```

## 7. VS Debugger

**Steps:** Open cplus.sln → F9 (breakpoints) → F5 (debug) → F10/F11 (step) → Ctrl+Alt+C (call stack)

## 8. Isolate (Binary Search)

```
Func A? YES → First half? NO → Second half? YES → Lines 45-50? YES → Line 47!
```

Comment out sections to isolate.

## 9. Root Cause (5 Whys)

1. Why X fail? → Y null  
2. Why Y null? → Z didn't init  
3. Why not init? → BeginPlay not called  
4. Why not called? → Component not added  
5. Why not added? → Blueprint not updated

**Common:** Null pointer, init order, delegate, UPROPERTY, signature, dependency

## 10. Fix Root Cause

**Fix root, not symptom!**

```cpp
// ✅ GOOD - Fix in BeginPlay
void BeginPlay() {
    Super::BeginPlay();
    Quest = LoadQuestFromData();
    if (!Quest) { UE_LOG(LogQuest, Error, TEXT("Failed to load!")); }
}

// Add defensive checks
void AcceptQuest(UQuestDefinition* Quest) {
    if (!Quest) { UE_LOG(LogQuest, Error, TEXT("Null")); return; }
    UQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
    if (!QuestSub) { UE_LOG(LogQuest, Error, TEXT("No subsystem")); return; }
    if (ActiveQuests.Contains(Quest->QuestID)) { UE_LOG(LogQuest, Warning, TEXT("Already active")); return; }
    ActiveQuests.Add(Quest->QuestID);
    QuestSub->AcceptQuest(Quest);
}
```

## 11. Test & Clean

**Test:** Normal, edge cases, stress, integration

```powershell
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
Start-Process "C:\ue5.7\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

**Verify:** Problem fixed, no regressions, performance OK

**Clean:** Remove excessive debug logs, keep important ones

## Checklist

- [ ] Understand (5min): Problem, reproduce, changes, logs
- [ ] Trace (15min): Find files, map flow, read all
- [ ] Debug (20min): Add logs, monitor, breakpoints
- [ ] Isolate (10min): Binary search to exact line
- [ ] Fix (15min): Root cause, defensive checks
- [ ] Verify (10min): Build, test, no regressions

## Patterns

**Null:** Crash → Add checks → Fix init  
**Not Called:** Nothing happens → Add logs → Bind delegate  
**Wrong Value:** Bad behavior → Log values → Fix calculation  
**Timing:** Intermittent → Check init order → Fix order

## Quick Commands

```
stat fps, stat unit, show collision, LogQuest LogLevel All
Get-ChildItem -Recurse -Include "*.cpp","*.h" | Select-String "TODO|FIXME"
```

**DO:** Read ALL files, trace COMPLETE flow, log EVERYWHERE, fix ROOT CAUSE  
**DON'T:** Skip files, assume, fix symptoms, forget cleanup

---

**v2.1 Ultra-optimized | 2026-01-14**
