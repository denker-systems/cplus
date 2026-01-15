---
name: ue-code-review
description: Perform systematic code review of Unreal Engine 5.7 C++ code following Epic's coding standards and project conventions
---

# UE 5.7 C++ Code Review

## Review Philosophy
- **Correctness first** - Does it work correctly?
- **Safety second** - Memory safe? Thread safe?
- **Maintainability third** - Can future devs understand it?
- **Performance last** - Optimize only when needed

## Review Checklist

### 1. Naming Conventions
- [ ] Classes use correct prefix (A, U, F, E, I)
- [ ] Functions use PascalCase
- [ ] Booleans use `b` prefix
- [ ] Variables are descriptive
- [ ] No abbreviations except well-known (ID, UI, AI)

### 2. UCLASS/UPROPERTY/UFUNCTION
- [ ] UPROPERTY on all UObject pointers
- [ ] Appropriate specifiers (EditAnywhere, BlueprintReadWrite, etc.)
- [ ] Category specified for Blueprint-exposed members
- [ ] No raw UObject pointers without UPROPERTY

### 3. Memory Safety
- [ ] TObjectPtr<> for UObject pointers
- [ ] TArray<> not std::vector
- [ ] FString not std::string
- [ ] No new/delete for UObjects
- [ ] No static UObject pointers

### 4. Code Structure
- [ ] .generated.h included last
- [ ] GENERATED_BODY() present
- [ ] Super:: called in overrides
- [ ] Const correctness maintained
- [ ] Forward declarations where possible

### 5. Documentation
- [ ] Class has Doxygen comment
- [ ] Public methods documented
- [ ] Complex logic explained
- [ ] @see tags for related classes
- [ ] Comments in English

### 6. Error Handling
- [ ] Null checks before pointer use
- [ ] IsValid() for UObjects
- [ ] check/ensure for assertions
- [ ] Graceful fallbacks

### 7. Performance
- [ ] No unnecessary Tick
- [ ] Expensive operations cached
- [ ] Delegates instead of polling
- [ ] Early returns for invalid states

## Common Issues

### Critical (Must Fix)
```cpp
// Missing UPROPERTY - will crash from GC
AActor* MyActor;  // ❌
UPROPERTY() TObjectPtr<AActor> MyActor;  // ✅

// Missing null check
Actor->DoSomething();  // ❌
if (IsValid(Actor)) { Actor->DoSomething(); }  // ✅

// Missing Super:: call
void BeginPlay() { DoStuff(); }  // ❌
void BeginPlay() { Super::BeginPlay(); DoStuff(); }  // ✅
```

### High Priority (Should Fix)
```cpp
// Wrong container type
std::vector<int> Values;  // ❌
TArray<int32> Values;  // ✅

// Polling instead of events
void Tick(float DT) { if (bChanged) Update(); }  // ❌
OnChanged.AddDynamic(this, &UMyClass::Update);  // ✅

// Hardcoded values
Damage = 50.0f;  // ❌
UPROPERTY(EditAnywhere) float Damage = 50.0f;  // ✅
```

### Medium Priority (Consider Fixing)
```cpp
// Missing const
float GetHealth() { return Health; }  // ❌
float GetHealth() const { return Health; }  // ✅

// Verbose null check
if (Actor != nullptr && Actor->IsValidLowLevel())  // ❌
if (IsValid(Actor))  // ✅
```

## Review Output Format

```markdown
## Code Review: [ClassName]

### Summary
[1-2 sentence overview of code quality]

### Critical Issues
1. [Issue description] - Line [X]
   - Current: `[code]`
   - Suggested: `[code]`

### Recommendations
1. [Recommendation]

### Positive Notes
- [What's done well]
```
