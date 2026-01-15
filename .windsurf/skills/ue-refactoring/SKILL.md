---
name: ue-refactoring
description: Systematic refactoring of Unreal Engine 5.7 C++ code with safety checks, incremental changes, and verification at each step
---

# UE 5.7 C++ Refactoring Methodology

## Core Principles
1. **Small steps** - One change at a time, verify after each
2. **Tests first** - Ensure tests exist before refactoring
3. **No behavior change** - Refactoring should not alter functionality
4. **Compile often** - Catch errors early

## Pre-Refactoring Checklist
- [ ] Code compiles without errors
- [ ] Existing tests pass
- [ ] Git status clean (commit or stash changes)
- [ ] Understand the code's purpose
- [ ] Identify all references to changed code

## Common Refactoring Patterns

### 1. Rename Class
```
Steps:
1. Rename .h and .cpp files
2. Update class name in both files
3. Update #include statements
4. Update forward declarations
5. Update UCLASS and .generated.h
6. Search for old name in entire codebase
7. Update Blueprints referencing the class
8. Full rebuild (header change)
9. Verify in editor
```

### 2. Extract Method
```cpp
// BEFORE: Long function
void ProcessQuest()
{
    // 20 lines of validation
    // 30 lines of processing
    // 10 lines of cleanup
}

// AFTER: Extracted methods
void ProcessQuest()
{
    if (!ValidateQuest()) return;
    DoProcessing();
    Cleanup();
}

bool ValidateQuest() { /* 20 lines */ }
void DoProcessing() { /* 30 lines */ }
void Cleanup() { /* 10 lines */ }
```

### 3. Extract Component
```
When a class has too many responsibilities:
1. Identify cohesive functionality group
2. Create new UActorComponent
3. Move relevant UPROPERTY/UFUNCTION
4. Add component to owning actor
5. Update references to use component
6. Verify all functionality intact
```

### 4. Replace Inheritance with Composition
```cpp
// BEFORE: Deep inheritance
class ASpecialEnemy : public AEnemy : public ACharacter { }

// AFTER: Component-based
class AEnemy : public ACharacter
{
    UPROPERTY()
    USpecialBehaviorComponent* SpecialBehavior;
};
```

### 5. Introduce Interface
```
When multiple classes share behavior:
1. Create UINTERFACE with shared methods
2. Implement interface on each class
3. Replace type checks with interface calls
4. Update callers to use interface
```

### 6. Replace Magic Numbers
```cpp
// BEFORE
if (Health < 25.0f) { /* low health logic */ }

// AFTER
static constexpr float kLowHealthThreshold = 25.0f;
if (Health < kLowHealthThreshold) { /* low health logic */ }
```

## Refactoring Workflow

### Step 1: Prepare
```powershell
# Verify clean state
git status
# Create feature branch
git checkout -b refactor/[description]
```

### Step 2: Execute (per change)
```
1. Make single focused change
2. Compile (Ctrl+Alt+F11 or full rebuild)
3. Run affected tests
4. Verify in editor if needed
5. Commit with descriptive message
```

### Step 3: Verify
```
1. Full rebuild
2. All tests pass
3. PIE works correctly
4. No new warnings
5. Code review
```

## Safe Search and Replace

### Find All References
- **Visual Studio:** Right-click → Find All References
- **Windsurf:** Grep search with context

### Replace Strategy
```powershell
# Search for usages first
grep -rn "OldClassName" Source/

# Verify each replacement location
# Make changes file by file
# Compile after each file
```

## Danger Zones

### High Risk Changes
- Renaming UPROPERTY (breaks serialization)
- Changing function signatures (breaks Blueprints)
- Removing virtual methods (breaks subclasses)
- Changing enum values (breaks saved data)

### Mitigation
```cpp
// Use DEPRECATED macro when removing
UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction, DeprecationMessage="Use NewMethod instead"))
void OldMethod();

// Add redirectors in DefaultEngine.ini
[CoreRedirects]
+ClassRedirects=(OldName="OldClass", NewName="NewClass")
+PropertyRedirects=(OldName="OldProp", NewName="NewProp")
```

## Post-Refactoring Checklist
- [ ] All tests pass
- [ ] No new compiler warnings
- [ ] Editor starts without errors
- [ ] PIE works correctly
- [ ] Blueprints compile
- [ ] Documentation updated
- [ ] Commit message describes change
