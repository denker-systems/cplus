---
name: ue-class-creation
description: Systematically create new Unreal Engine 5.7 C++ classes with proper architecture, naming conventions, and UCLASS/UPROPERTY/UFUNCTION specifiers
---

# UE 5.7 C++ Class Creation

## Pre-Creation Checklist
1. **Determine class type** - Actor (A), UObject (U), Component, Interface (I), Struct (F), Enum (E)
2. **Identify parent class** - AActor, ACharacter, UActorComponent, UGameInstanceSubsystem, etc.
3. **Choose target system** - QuestSystem, Variant_Shooter, Variant_Horror, Core, UI
4. **Plan dependencies** - What modules/headers will be needed?

## Class Type Selection

| Type | Prefix | Parent | Use Case |
|------|--------|--------|----------|
| Actor | A | AActor | Placeable in world |
| Character | A | ACharacter | Movable characters |
| Component | U | UActorComponent | Attachable functionality |
| Subsystem | U | UGameInstanceSubsystem | Global managers |
| DataAsset | U | UPrimaryDataAsset | Data-driven design |
| Interface | I | UInterface | Polymorphic contracts |
| Struct | F | - | Data containers |
| Enum | E | - | State/type enumerations |

## File Structure Template

### Header (.h)
```cpp
#pragma once

#include "CoreMinimal.h"
#include "[ParentHeader].h"
#include "[ClassName].generated.h"

/**
 * [Brief description of class purpose]
 * 
 * [Detailed usage explanation]
 * 
 * @see [RelatedClass1]
 * @see [RelatedClass2]
 */
UCLASS([Specifiers])
class CPLUS_API [AUF][ClassName] : public [ParentClass]
{
    GENERATED_BODY()

public:
    [AUF][ClassName]();

    // [Interface] interface
    virtual void [Method]_Implementation() override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "[Category]")
    [Type] [PropertyName];
};
```

### Implementation (.cpp)
```cpp
#include "[ClassName].h"

[AUF][ClassName]::[AUF][ClassName]()
{
    PrimaryActorTick.bCanEverTick = false; // Event-driven preferred
}

void [AUF][ClassName]::BeginPlay()
{
    Super::BeginPlay();
}
```

## Step-by-Step Process

### Step 1: Create Header File
1. Add `#pragma once`
2. Include `CoreMinimal.h`
3. Include parent class header
4. Include `.generated.h` (MUST be last)
5. Add class documentation (Doxygen style)
6. Define UCLASS with appropriate specifiers
7. Add GENERATED_BODY()
8. Declare constructor
9. Override virtual methods as needed
10. Add UPROPERTY members

### Step 2: Create Implementation File
1. Include the header file
2. Include additional dependencies
3. Implement constructor
4. Call Super:: on overrides
5. Implement all declared methods

### Step 3: Update Build System
1. Verify module in `cplus.Build.cs` PublicIncludePaths
2. Add new module dependencies if needed

### Step 4: Compile & Verify
1. Live Coding (Ctrl+Alt+F11) for .cpp changes
2. Full rebuild for .h changes
3. Check Output Log for errors

## Common UCLASS Specifiers

- `Blueprintable` - Can create Blueprint subclass
- `Abstract` - Cannot instantiate directly
- `ClassGroup=(Custom)` - For components
- `meta=(BlueprintSpawnableComponent)` - Component spawnable
- `BlueprintType` - Can use as variable type

## Common UPROPERTY Specifiers

- `EditAnywhere, BlueprintReadWrite` - Full access
- `VisibleAnywhere, BlueprintReadOnly` - Read-only
- `EditDefaultsOnly` - Only in defaults
- `Transient` - Not saved
- `Instanced` - For inline UObject editing

## Common UFUNCTION Specifiers

- `BlueprintCallable` - Can call from Blueprint
- `BlueprintPure` - No exec pin, no side effects
- `BlueprintNativeEvent` - C++ with BP override
- `BlueprintImplementableEvent` - BP only implementation
