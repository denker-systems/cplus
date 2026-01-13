# Unreal Engine New Feature Workflow

> **Destination:** `.windsurf/workflows/new-feature.md`

---
description: Develop new features for Unreal Engine 5.7 C++
auto_execution_mode: 1
---

# New Feature Workflow

> Complete process for developing new features in Unreal Engine

## 1. Planning and Design

### Define Feature
```powershell
Write-Host "=== FEATURE PLANNING ===" -ForegroundColor Yellow
Write-Host "Feature Name: [Describe feature briefly]"
Write-Host "Type: [New System|Component|Gameplay|UI|AI]"
Write-Host "Scope: [quest|shooter|horror|core|ai|ui]"
Write-Host "Priority: [Critical|High|Medium|Low]"
```

### Design Questions
- [ ] What should the feature do?
- [ ] Which Unreal classes are needed?
- [ ] Are new interfaces needed?
- [ ] Are DataAssets needed?
- [ ] How does it integrate with existing systems?
- [ ] What dependencies are needed?

### Architecture
```
Feature: [Name]
├── Classes
│   ├── U[Feature]Component.h/cpp      # Main component
│   ├── I[Feature]Interface.h/cpp      # Interface (if needed)
│   └── U[Feature]Data.h/cpp           # DataAsset (if needed)
├── Blueprints
│   └── BP_[Feature].uasset            # Blueprint implementation
└── Content
    └── Data/[Feature]/                 # Data assets
```

---

## 2. Setup - Create Branch and Structure

### Create Branch
```powershell
# Create feature branch
git checkout -b feature/[feature-name]

# Examples:
git checkout -b feature/quest-rewards
git checkout -b feature/ai-behavior-tree
git checkout -b feature/horror-sound-system
```

### Create File Structure
```powershell
# Create folders
New-Item -ItemType Directory -Path "Source/cplus/[System]/[Feature]" -Force
New-Item -ItemType Directory -Path "Content/Blueprints/[Feature]" -Force
New-Item -ItemType Directory -Path "Content/Data/[Feature]" -Force

# Example for Quest Rewards:
New-Item -ItemType Directory -Path "Source/cplus/QuestSystem/Rewards" -Force
New-Item -ItemType Directory -Path "Content/Blueprints/Quests/Rewards" -Force
New-Item -ItemType Directory -Path "Content/Data/Quests/Rewards" -Force
```

---

## 3. Implementation - Create Classes

### Template for UActorComponent
```cpp
// [Feature]Component.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "[Feature]Component.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API U[Feature]Component : public UActorComponent
{
    GENERATED_BODY()

public:
    U[Feature]Component();

    // Component lifecycle
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Public API
    UFUNCTION(BlueprintCallable, Category = "[Feature]")
    void [Method]();

protected:
    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[Feature]")
    float [Property] = 0.0f;

    UPROPERTY(BlueprintAssignable, Category = "[Feature]")
    F[Event] On[Event];
};
```

### Template for UCLASS
```cpp
// [Feature].h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "[Feature].generated.h"

UCLASS(BlueprintType, Blueprintable)
class CPLUS_API U[Feature] : public UObject
{
    GENERATED_BODY()

public:
    U[Feature]();

    // UFUNCTIONs
    UFUNCTION(BlueprintCallable, Category = "[Feature]")
    void [Method]();

    // UPROPERTYs
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[Feature]")
    TArray<FName> [Array];
};
```

### Template for Interface
```cpp
// I[Feature].h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I[Feature].generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class U[Feature] : public UInterface
{
    GENERATED_BODY()
};

class CPLUS_API I[Feature]
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "[Feature]")
    void [Method]();
};
```

---

## 4. Implementation Steps

### Step 1: Create Header Files
```powershell
# Create h files
New-Item -ItemType File -Path "Source/cplus/[System]/[Feature]/[Feature].h"
New-Item -ItemType File -Path "Source/cplus/[System]/[Feature]/[Feature]Component.h"
```

### Step 2: Implement Headers
- Add UCLASS/USTRUCT/UENUM
- Add UPROPERTY/UFUNCTION
- Add GENERATED_BODY()
- Include necessary headers

### Step 3: Create Implementation Files
```powershell
# Create cpp files
New-Item -ItemType File -Path "Source/cplus/[System]/[Feature]/[Feature].cpp"
New-Item -ItemType File -Path "Source/cplus/[System]/[Feature]/[Feature]Component.cpp"
```

### Step 4: Implement C++
- Constructor
- BeginPlay()
- Public methods
- Event handlers

### Step 5: Update .Build.cs
```csharp
// In cplus.Build.cs
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "CoreUObject", 
    "Engine",
    // Add new dependencies here
    "GameplayTags",  // If GameplayTags used
    "UMG"           // If UI used
});
```

---

## 5. Blueprint Integration

### Create Blueprint Base
1. Open Unreal Editor
2. Content Browser → Right Click → Blueprint Class
3. Choose base class (e.g., Actor)
4. Save as `BP_[Feature]`

### Add Components
1. Open Blueprint
2. Add Component → Search your component
3. Configure properties
4. Connect events

### Test Blueprint
1. Compile Blueprint
2. Place in level
3. Test functionality

---

## 6. Testing Strategy

### Unit Tests (C++)
```cpp
// In [Feature]Test.h
UCLASS()
class CPLUS_API U[Feature]Test : public UObject
{
public:
    UFUNCTION(BlueprintCallable, Category = "Test")
    bool Test[Feature]Functionality();
};
```

### Integration Tests
- [ ] Component works on Actor
- [ ] Events broadcast correctly
- [ ] DataAssets load correctly
- [ ] Blueprint integration works
- [ ] Performance OK (stat unit)

### Test Checklist
```powershell
Write-Host "=== TESTING CHECKLIST ===" -ForegroundColor Yellow
Write-Host "□ Build works (Development)"
Write-Host "□ Editor starts without crash"
Write-Host "□ Component visible in Blueprint"
Write-Host "□ Properties editable in editor"
Write-Host "□ Functions callable from Blueprint"
Write-Host "□ Events broadcast correctly"
Write-Host "□ Performance OK (stat fps)"
```

---

## 7. Documentation

### Code Documentation
```cpp
/**
 * [Feature] Component for [purpose]
 * 
 * This component handles [what it does]
 * and integrates with [other systems].
 * 
 * @see U[RelatedClass]
 */
UCLASS()
class U[Feature]Component : public UActorComponent
{
    /**
     * [Description of what this function does]
     * @param [ParamName] [Description of parameter]
     * @return [Description of return value]
     */
    UFUNCTION(BlueprintCallable)
    void [Function](float [Param]);
};
```

### Update CHANGELOG.md
```markdown
## [Unreleased]

### Added
- [Feature] component for [purpose]
- [Feature] interface for [integration]
- [Feature] data assets for [configuration]

### Changed
- Updated [RelatedSystem] to support [Feature]
```

### Update ROADMAP.md
```markdown
### Current Sprint
- [ ] [Feature] implementation
- [ ] [Feature] testing
- [ ] [Feature] documentation
```

---

## 8. Code Review

### Self-Review Checklist
- [ ] Code follows Unreal coding standards
- [ ] All UPROPERTY have correct specifiers
- [ ] All UFUNCTION have correct specifiers
- [ ] No memory leak (TObjectPtr for UPROPERTY)
- [ ] No unnecessary Tick
- [ ] Error handling with check/ensure
- [ ] Performance considerations (cache casts, etc)
- [ ] Documentation added

### Review Questions
- Is class Single Responsibility?
- Is it testable?
- Is it extensible?
- Is it performant?
- Is it Blueprint-friendly?

---

## 9. Feature Examples

### Example 1: Quest Reward System
```
Feature: Quest Rewards
Type: New System
Scope: quest

Classes:
- UQuestRewardComponent (on player)
- IQuestReward (interface)
- UQuestRewardData (DataAsset)

Integration:
- Connect to QuestSubsystem
- Trigger on quest completion
- Support different reward types
```

### Example 2: AI Behavior Tree Extension
```
Feature: Advanced AI Behavior
Type: AI System
Scope: ai

Classes:
- UAdvancedAIBehaviorComponent
- IAIBehaviorInterface
- UAIBehaviorData

Integration:
- Extend existing AIController
- Add new behavior tree nodes
- Support dynamic behavior switching
```

---

## 10. Common Patterns

### Component Pattern
```cpp
// GOOD - Modular component
UCLASS()
class UMyFeatureComponent : public UActorComponent
{
    // Self-contained feature
    void ActivateFeature();
    void DeactivateFeature();
};

// BAD - Monolithic class
UCLASS()
class AMyActor : public AActor
{
    // Everything in one class
    void HandleFeature1();
    void HandleFeature2();
    void HandleFeature3();
};
```

### Event-Driven Pattern
```cpp
// GOOD - Event driven
UPROPERTY(BlueprintAssignable)
FOnFeatureEvent OnFeatureActivated;

// Activate via event
OnFeatureActivated.Broadcast();

// BAD - Polling
virtual void Tick(float DeltaTime) override
{
    if (bShouldActivateFeature) // Check every frame
    {
        ActivateFeature();
    }
}
```

### Data-Driven Pattern
```cpp
// GOOD - DataAsset
UCLASS()
class UFeatureData : public UPrimaryDataAsset
{
    UPROPERTY(EditAnywhere)
    float FeatureParameter1;
    
    UPROPERTY(EditAnywhere)
    bool FeatureOption1;
};

// BAD - Hardcoded
const float PARAMETER_1 = 5.0f;
const bool OPTION_1 = true;
```

---

## 11. Build and Deploy

### Build Verification
```powershell
# Build Development
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Verify success
if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful" -ForegroundColor Green
} else {
    Write-Host "❌ Build failed" -ForegroundColor Red
}
```

### Test in Editor
1. Start Unreal Editor
2. Create test level
3. Add test actors with components
4. Verify functionality
5. Check performance with stat commands

---

## 12. Git Commit

### Commit Message Format
```bash
feat(scope): Add [Feature] system

- Implement [Feature]Component for [purpose]
- Add [Feature]Interface for [integration]
- Create [Feature]Data assets for configuration
- Integrate with [RelatedSystem]

Closes #[issue-number]
```

### Commit Changes
```powershell
# Stage all feature files
git add Source/cplus/[System]/[Feature]/
git add Content/[Feature]/

# Commit
git commit -m "feat(scope): Add [Feature] system"
```

---

## Quick Reference

### File Templates
```cpp
// Component
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class U[Feature]Component : public UActorComponent

// Class
UCLASS(BlueprintType, Blueprintable)
class U[Feature] : public UObject

// Interface
UINTERFACE(MinimalAPI, Blueprintable)
class I[Feature] : public UInterface

// DataAsset
UCLASS(BlueprintType)
class U[Feature]Data : public UPrimaryDataAsset
```

### Common UPROPERTY Specifiers
```cpp
EditAnywhere        // Editable in editor and Blueprint
BlueprintReadWrite  // Get/set in Blueprint
VisibleAnywhere     // Read-only in Blueprint
BlueprintReadOnly   // Get only in Blueprint
Instanced          // Create instance in DataAsset
```

### Common UFUNCTION Specifiers
```cpp
BlueprintCallable   // Call from Blueprint
BlueprintPure       // Pure function (no exec pin)
BlueprintImplementableEvent  // Implement in BP only
BlueprintNativeEvent         // C++ default, BP override
```

---

## Feature Development Checklist

### Planning
- [ ] Feature requirements defined
- [ ] Architecture designed
- [ ] Dependencies identified
- [ ] Branch created

### Implementation
- [ ] Header files created
- [ ] Implementation files created
- [ ] .Build.cs updated
- [ ] Code compiles

### Integration
- [ ] Component works on Actor
- [ ] Blueprint integration tested
- [ ] Events working
- [ ] DataAssets loading

### Testing
- [ ] Unit tests passing
- [ ] Integration tests passing
- [ ] Performance OK
- [ ] No memory leaks

### Documentation
- [ ] Code documented
- [ ] CHANGELOG updated
- [ ] ROADMAP updated
- [ ] Examples created

### Final
- [ ] Code review complete
- [ ] Build verification
- [ ] Git commit with proper message
- [ ] Ready for merge

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
