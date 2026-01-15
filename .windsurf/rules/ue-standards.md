---
trigger: always_on
description: Unreal Engine 5.7 C++ coding standards
---

# UE 5.7 C++ Standards

<naming_conventions>
Classes use prefix + PascalCase:
- A = Actor (AShooterCharacter, AShooterNPC)
- U = UObject/Component (UQuestSubsystem, UQuestTrackerComponent)
- F = Struct (FQuestObjective, FQuestReward)
- E = Enum (EQuestState, EQuestType)
- I = Interface (IQuestInteractable, IQuestKillable)
- T = Template (TArray, TMap)

Variables:
- PascalCase for UPROPERTY (QuestID, CurrentCount)
- b prefix for booleans (bIsMandatory, bIsComplete)
- k prefix for constants (kMaxQuestCount)

Functions: PascalCase (AcceptQuest, CompleteQuest)
</naming_conventions>

<memory_management>
- Use TObjectPtr<> for UPROPERTY UObject pointers
- Use TArray<> not std::vector
- Use TMap<> not std::map
- Use FString not std::string
- Use FName for constant strings (fast comparison)
- ALWAYS use UPROPERTY() for UObject references (GC!)
- NEVER use raw new/delete for UObjects
</memory_management>

<forbidden_practices>
- std::string, std::vector, std::map
- new/delete for UObjects (use NewObject, SpawnActor)
- Raw UObject* without UPROPERTY (will be garbage collected!)
- Static UObject pointers
- dynamic_cast (use Cast<>)
- Exceptions (Unreal doesn't use them)
- Unnecessary Tick functions
- Hardcoded data (use DataAssets)
- Swedish comments
</forbidden_practices>

<required_patterns>
- GENERATED_BODY() in all UCLASS/USTRUCT
- .generated.h as LAST include
- Super::BeginPlay() in overrides
- IsValid(Ptr) before UObject pointer use
- Cast<T>() for type casting
</required_patterns>

<file_structure>
Header order:
1. #pragma once
2. CoreMinimal.h
3. Parent class header
4. Other UE headers
5. Project headers
6. [ClassName].generated.h (LAST!)

Implementation:
1. Own header
2. Other includes
3. Constructor
4. BeginPlay/EndPlay
5. Public methods
6. Protected methods
7. Private methods
</file_structure>

<build_system>
Live Coding: Ctrl+Alt+F11 (cpp changes only)
Full rebuild: Required for .h changes, new UCLASS/UPROPERTY/UFUNCTION
Config: Development for daily work
</build_system>