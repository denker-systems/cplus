---
trigger: always_on
---
# Unreal Engine C++ Coding Standards

> **Destination:** `.windsurf/rules/coding-standards.md`

---
trigger: always_on
description: Unreal Engine 5.7 C++ coding standards
---

# Coding Standards

> Unreal Engine C++ standards for cplus project

## Naming Conventions

| Type | Convention | Example |
|------|------------|---------|
| **Classes** | Prefix + PascalCase | `AShooterCharacter`, `UQuestTask` |
| **Interfaces** | I + PascalCase | `IQuestInteractable` |
| **Structs** | F + PascalCase | `FQuestObjective` |
| **Enums** | E + PascalCase | `EQuestState` |
| **Functions** | PascalCase | `AcceptQuest()` |
| **Variables** | PascalCase (UPROPERTY) | `CurrentCount` |
| **Bool vars** | b prefix | `bIsMandatory` |
| **Member vars** | No prefix or m_ | `QuestID` or `m_Position` |
| **Constants** | k prefix | `kMaxHealth` |
| **Namespaces** | Avoid (Unreal doesn't use) | - |

### Class Prefixes (Unreal Standard)

| Prefix | Type | Example |
|--------|------|---------|
| `A` | Actor (placeable) | `AShooterNPC` |
| `U` | UObject, Component, Widget | `UQuestTrackerComponent` |
| `F` | Struct, Plain C++ | `FQuestReward` |
| `E` | Enum | `EQuestCompletionRule` |
| `I` | Interface | `IQuestKillable` |
| `T` | Template | `TArray`, `TMap` |

---

## File Structure

### Header Files (.h)

```cpp
// Copyright Notice (optional)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuestInteractable.h"
#include "ShooterNPC.generated.h"

/**
 * AI-controlled enemy NPC with quest integration
 */
UCLASS()
class CPLUS_API AShooterNPC : public ACharacter, public IQuestInteractable
{
    GENERATED_BODY()

public:
    // Constructor
    AShooterNPC();

    // AActor interface
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // IQuestInteractable interface
    virtual void Interact_Implementation(AActor* Interactor) override;

    // Public API
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TakeDamage(float Damage);

protected:
    // Protected methods
    void Die();

private:
    // Private methods
    void UpdateAI();

    // UPROPERTY members
    UPROPERTY(EditAnywhere, Category = "Quest")
    class UQuestGiverComponent* QuestGiver;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float Health = 100.0f;

    // Non-UPROPERTY members
    bool bIsDead = false;
};
```

### Implementation Files (.cpp)

```cpp
#include "ShooterNPC.h"
#include "QuestGiverComponent.h"
#include "Components/CapsuleComponent.h"

AShooterNPC::AShooterNPC()
{
    PrimaryActorTick.bCanEverTick = true;
    
    QuestGiver = CreateDefaultSubobject<UQuestGiverComponent>(TEXT("QuestGiver"));
}

void AShooterNPC::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialization
}

void AShooterNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    UpdateAI();
}

void AShooterNPC::Interact_Implementation(AActor* Interactor)
{
    if (QuestGiver)
    {
        // Quest interaction logic
    }
}
```

---

## UCLASS, USTRUCT, UENUM

### UCLASS Specifiers

```cpp
// Blueprint-creatable actor
UCLASS(Blueprintable)
class CPLUS_API AMyActor : public AActor { };

// Abstract base class
UCLASS(Abstract)
class CPLUS_API UQuestTask : public UTool { }

// Component
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestTrackerComponent : public UActorComponent { }

// DataAsset
UCLASS(BlueprintType)
class CPLUS_API UQuestDefinition : public UPrimaryDataAsset { };
```

### UPROPERTY Specifiers

```cpp
// Editable in editor, visible in Blueprint
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
FName QuestID;

// Visible but not editable
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
int32 CurrentCount;

// Blueprint-only (not visible in editor)
UPROPERTY(BlueprintReadOnly, Category = "Quest")
EQuestState State;

// Instanced (for components in DataAssets)
UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
TArray<TObjectPtr<UQuestTask>> Tasks;

// Transient (not saved)
UPROPERTY(Transient)
float CachedValue;
```

### UFUNCTION Specifiers

```cpp
// Blueprint callable
UFUNCTION(BlueprintCallable, Category = "Quest")
void AcceptQuest(UQuestDefinition* Quest);

// Blueprint pure (no exec pin)
UFUNCTION(BlueprintPure, Category = "Quest")
bool IsComplete() const;

// Blueprint implementable event
UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
void OnQuestStarted(FName QuestID);

// Blueprint native event (C++ + BP override)
UFUNCTION(BlueprintNativeEvent, Category = "Quest")
void Interact(AActor* Interactor);
// Implementation:
void Interact_Implementation(AActor* Interactor) { }
```

### USTRUCT

```cpp
USTRUCT(BlueprintType)
struct FQuestObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ObjectiveDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TObjectPtr<UQuestTask>> Tasks;

    // Non-UPROPERTY members allowed
    bool IsComplete() const;
};
```

### UENUM

```cpp
UENUM(BlueprintType)
enum class EQuestState : uint8
{
    NotStarted  UMETA(DisplayName = "Not Started"),
    Active      UMETA(DisplayName = "Active"),
    Completed   UMETA(DisplayName = "Completed"),
    Failed      UMETA(DisplayName = "Failed")
};
```

---

## Memory Management

### Unreal Smart Pointers

```cpp
// UPROPERTY - Garbage collected
UPROPERTY()
TObjectPtr<UQuestDefinition> Quest;

// TArray - Unreal's dynamic array
UPROPERTY()
TArray<FName> ActiveQuestIDs;

// TMap - Unreal's hash map
UPROPERTY()
TMap<FName, FActiveQuest> ActiveQuests;

// Raw pointer for non-owning reference (OK if UPROPERTY)
UPROPERTY()
AActor* TargetActor;

// TSharedPtr - For non-UObject shared ownership (rare)
TSharedPtr<FMyData> SharedData;

// TWeakObjectPtr - Weak reference to UObject
TWeakObjectPtr<AActor> WeakActor;
```

### Object Creation

```cpp
// Create component in constructor
UQuestTrackerComponent* Tracker = CreateDefaultSubobject<UQuestTrackerComponent>(TEXT("QuestTracker"));

// Create UObject at runtime
UQuestDefinition* Quest = NewObject<UQuestDefinition>(this);

// Spawn actor
AShooterNPC* NPC = GetWorld()->SpawnActor<AShooterNPC>(NPCClass, Location, Rotation);
```

---

## Delegates and Events

### Declare Delegates

```cpp
// Single-cast delegate
DECLARE_DELEGATE_OneParam(FOnQuestAccepted, FName);

// Multi-cast delegate (Blueprint-compatible)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnQuestCompleted, 
    FName, QuestID, 
    const FQuestReward&, Rewards
);
```

### Use Delegates

```cpp
// In class
UPROPERTY(BlueprintAssignable, Category = "Quest")
FOnQuestCompleted OnQuestCompleted;

// Broadcast
OnQuestCompleted.Broadcast(QuestID, Rewards);

// Bind (C++)
OnQuestCompleted.AddDynamic(this, &UMyClass::HandleQuestCompleted);

// Unbind
OnQuestCompleted.RemoveDynamic(this, &UMyClass::HandleQuestCompleted);
```

---

## Interfaces

### Define Interface

```cpp
// QuestInteractable.h
UINTERFACE(MinimalAPI, Blueprintable)
class UQuestInteractable : public UInterface
{
    GENERATED_BODY()
};

class CPLUS_API IQuestInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
    void Interact(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
    bool CanInteract(AActor* Interactor) const;
};
```

### Implement Interface

```cpp
// ShooterNPC.h
class AShooterNPC : public ACharacter, public IQuestInteractable
{
    // IQuestInteractable
    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual bool CanInteract_Implementation(AActor* Interactor) const override;
};

// ShooterNPC.cpp
void AShooterNPC::Interact_Implementation(AActor* Interactor)
{
    // Implementation
}
```

---

## Logging

### Log Categories

```cpp
// Define in .h or .cpp
DEFINE_LOG_CATEGORY_STATIC(LogQuest, Log, All);

// Use
UE_LOG(LogQuest, Log, TEXT("Quest accepted: %s"), *QuestID.ToString());
UE_LOG(LogQuest, Warning, TEXT("Quest not found: %s"), *QuestID.ToString());
UE_LOG(LogQuest, Error, TEXT("Failed to load quest: %s"), *QuestID.ToString());

// With formatting
UE_LOG(LogQuest, Log, TEXT("Progress: %d/%d"), CurrentCount, TargetCount);
```

### Common Format Specifiers

| Type | Specifier | Example |
|------|-----------|---------|
| int32 | `%d` | `%d` |
| float | `%f` | `%.2f` |
| FString | `%s` | `*MyString` |
| FName | `%s` | `*MyName.ToString()` |
| bool | `%s` | `bValue ? TEXT("true") : TEXT("false")` |

---

## Const Correctness

```cpp
// Const method - doesn't modify state
UFUNCTION(BlueprintPure, Category = "Quest")
bool IsComplete() const;

// Const reference parameter - avoids copy
void SetQuestData(const FQuestObjective& Objective);

// Const pointer parameter
void ProcessQuest(const UQuestDefinition* Quest);
```

---

## Forbidden Practices (Unreal)

### DON'T

- ❌ `new` / `delete` (use NewObject, SpawnActor)
- ❌ `std::string` (use FString)
- ❌ `std::vector` (use TArray)
- ❌ `std::map` (use TMap)
- ❌ Raw pointers to UObjects without UPROPERTY
- ❌ Static UObject pointers
- ❌ Multiple inheritance (except interfaces)
- ❌ RTTI (`dynamic_cast`) - use Cast<>
- ❌ Exceptions (Unreal doesn't use them)

### DO

- ✅ Use UPROPERTY for all UObject references
- ✅ Use TObjectPtr<> for UObject pointers
- ✅ Use FString, FName, FText
- ✅ Use TArray, TMap, TSet
- ✅ Use Cast<> for type casting
- ✅ Use check() for assertions
- ✅ Use ensure() for soft assertions

---

## Type Casting

```cpp
// Cast UObject to specific type
AShooterNPC* NPC = Cast<AShooterNPC>(Actor);
if (NPC)
{
    // Safe to use
}

// Interface cast
IQuestInteractable* Interactable = Cast<IQuestInteractable>(Actor);

// Checked cast (crashes if fails - use only when certain)
AShooterNPC* NPC = CastChecked<AShooterNPC>(Actor);
```

---

## Assertions

```cpp
// check() - Crashes in all builds if false
check(Quest != nullptr);

// checkf() - With formatted message
checkf(CurrentCount <= TargetCount, TEXT("Count overflow: %d > %d"), CurrentCount, TargetCount);

// ensure() - Logs error but continues (once per call site)
if (ensure(QuestSubsystem != nullptr))
{
    // Use QuestSubsystem
}

// ensureMsgf() - With formatted message
ensureMsgf(Tasks.Num() > 0, TEXT("Quest has no tasks: %s"), *QuestID.ToString());

// verify() - Like check() but expression always executes
verify(QuestSubsystem->AcceptQuest(Quest));
```

---

## Best Practices

### Component Pattern

```cpp
// GOOD - Modular with components
UCLASS()
class AShooterNPC : public ACharacter
{
    UPROPERTY(VisibleAnywhere)
    UQuestGiverComponent* QuestGiver;
    
    UPROPERTY(VisibleAnywhere)
    UQuestTargetComponent* QuestTarget;
};

// BAD - Monolithic class
class AShooterNPC : public ACharacter
{
    // All quest logic hardcoded in NPC
    void OfferQuest();
    void OnKilled();
    TArray<UQuestDefinition*> AvailableQuests;
};
```

### Tick Optimization

```cpp
// BAD - Unnecessary tick
UCLASS()
class AMyActor : public AActor
{
    AMyActor() { PrimaryActorTick.bCanEverTick = true; }
    
    virtual void Tick(float DeltaTime) override
    {
        // Runs every frame!
    }
};

// GOOD - Event-driven
UCLASS()
class AMyActor : public AActor
{
    AMyActor() { PrimaryActorTick.bCanEverTick = false; }
    
    UPROPERTY(BlueprintAssignable)
    FOnSomethingHappened OnSomethingHappened;
    
    void TriggerEvent()
    {
        OnSomethingHappened.Broadcast();
    }
};
```

---

## Code Organization

### Include Order

```cpp
// 1. Corresponding header
#include "ShooterNPC.h"

// 2. Unreal headers
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

// 3. Project headers
#include "QuestGiverComponent.h"
#include "ShooterAIController.h"

// 4. Third-party (if any)
// #include <ThirdParty/Library.h>

// 5. Standard library (avoid if possible)
// #include <vector>
```

### Forward Declarations

```cpp
// In .h - Use forward declarations when possible
class UQuestDefinition;
class UQuestGiverComponent;

// In .cpp - Include full headers
#include "QuestDefinition.h"
#include "QuestGiverComponent.h"
```

---

## Quick Reference

### Common Unreal Types

```cpp
FString     // Mutable string
FName       // Immutable string (fast comparison)
FText       // Localized text
int32       // 32-bit integer
float       // 32-bit float
bool        // Boolean (use b prefix for members)
TArray<T>   // Dynamic array
TMap<K,V>   // Hash map
TSet<T>     // Hash set
```

### Common Macros

```cpp
GENERATED_BODY()            // Required in UCLASS/USTRUCT
UPROPERTY()                 // Mark property for reflection
UFUNCTION()                 // Mark function for reflection
UCLASS()                    // Mark class as UObject
USTRUCT()                   // Mark struct for reflection
UENUM()                     // Mark enum for reflection
UINTERFACE()                // Mark interface
TEXT("string")              // Create FString literal
```

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
