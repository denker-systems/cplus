---
name: ue-debugging
description: Methodically debug Unreal Engine 5.7 C++ issues using systematic root cause analysis, logging, and UE debugging tools
---

# UE 5.7 C++ Debugging Methodology

## Debugging Principles
1. **Reproduce consistently** - Identify exact steps to trigger issue
2. **Isolate the problem** - Narrow down to specific system/class
3. **Understand before fixing** - Root cause, not symptoms
4. **Verify the fix** - Ensure fix doesn't break other systems

## Phase 1: Information Gathering

### Collect Error Data
```
1. Copy exact error message from Output Log
2. Note crash callstack if available
3. Identify when issue started (last known working state)
4. Document reproduction steps
```

### Check Common Locations
- **Output Log** - Window → Developer Tools → Output Log
- **Message Log** - Window → Developer Tools → Message Log
- **Visual Logger** - ' key (apostrophe)
- **Crash logs** - Saved/Crashes/

## Phase 2: Systematic Analysis

### Error Type Classification

| Error Type | Symptoms | First Check |
|------------|----------|-------------|
| Nullptr crash | Access violation | UPROPERTY marked? Initialized in BeginPlay? |
| GC crash | Random crash after delay | Raw pointer to UObject? |
| Compile error | Build fails | .generated.h last? GENERATED_BODY()? |
| UHT error | Unrecognized type | Forward declare? Include missing? |
| Linker error | Unresolved external | Module in Build.cs? |
| Blueprint error | Node disconnected | C++ signature changed? |

### Common UE 5.7 Crash Causes

1. **Nullptr dereference**
   ```cpp
   // BAD
   Actor->DoSomething();
   
   // GOOD
   if (IsValid(Actor)) { Actor->DoSomething(); }
   ```

2. **Garbage collected pointer**
   ```cpp
   // BAD - will be GC'd
   AActor* MyActor;
   
   // GOOD - prevents GC
   UPROPERTY()
   TObjectPtr<AActor> MyActor;
   ```

3. **Missing Super:: call**
   ```cpp
   // BAD
   void BeginPlay() { /* code */ }
   
   // GOOD
   void BeginPlay() { Super::BeginPlay(); /* code */ }
   ```

4. **Constructor vs BeginPlay confusion**
   ```cpp
   // BAD - World doesn't exist in constructor
   AMyActor() { GetWorld()->SpawnActor(...); }
   
   // GOOD - World exists in BeginPlay
   void BeginPlay() { GetWorld()->SpawnActor(...); }
   ```

## Phase 3: Targeted Debugging

### Add Diagnostic Logging
```cpp
// At function entry
UE_LOG(LogTemp, Log, TEXT("%s: [FunctionName] called with Param=%s"), 
    *GetName(), *ParamValue.ToString());

// At suspicious locations
UE_LOG(LogTemp, Warning, TEXT("%s: Pointer is %s"), 
    *GetName(), Pointer ? TEXT("valid") : TEXT("NULL"));

// Before crash-prone code
if (!ensure(IsValid(Actor)))
{
    UE_LOG(LogTemp, Error, TEXT("%s: Actor is invalid!"), *GetName());
    return;
}
```

### Visual Studio Debugging
1. **Attach to Editor** - Debug → Attach to Process → UnrealEditor.exe
2. **Set breakpoints** - F9 on line
3. **Conditional breakpoints** - Right-click → Conditions
4. **Watch variables** - Debug → Windows → Watch

### Blueprint Debugging
1. **Enable debug** - Alt+Shift+D
2. **Add breakpoints** - Right-click node → Add Breakpoint
3. **Watch values** - Add to watch panel

## Phase 4: Fix Implementation

### Fix Verification Checklist
- [ ] Fix addresses root cause, not symptom
- [ ] No new warnings introduced
- [ ] Related systems still work
- [ ] Build succeeds in Development
- [ ] Editor starts without crash
- [ ] Reproduction steps no longer trigger issue

### Regression Prevention
```cpp
// Add defensive checks
check(Condition);           // Hard crash if false (dev builds)
ensure(Condition);          // Log + continue (once per callsite)
ensureMsgf(Cond, TEXT("...")); // Log with message
verify(Expression);         // Always executes, checks result
```

## Quick Reference: Log Categories

```cpp
// Define custom log category
DEFINE_LOG_CATEGORY_STATIC(LogQuest, Log, All);

// Use it
UE_LOG(LogQuest, Log, TEXT("Message"));
UE_LOG(LogQuest, Warning, TEXT("Warning"));
UE_LOG(LogQuest, Error, TEXT("Error"));
```

## Quick Reference: Console Commands

| Command | Purpose |
|---------|---------|
| `stat fps` | Show FPS |
| `stat unit` | Show frame breakdown |
| `stat game` | Show game thread time |
| `stat memory` | Memory usage |
| `obj list class=[ClassName]` | List all instances |
| `log [Category] [Level]` | Set log verbosity |
