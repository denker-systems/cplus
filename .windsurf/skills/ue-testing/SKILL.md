---
name: ue-testing
description: Systematic testing procedures for Unreal Engine 5.7 C++ code including unit tests, integration tests, and in-editor verification
---

# UE 5.7 C++ Testing Methodology

## Testing Pyramid
1. **Unit Tests** - Test individual functions/classes in isolation
2. **Integration Tests** - Test system interactions
3. **Editor Tests** - Manual verification in-editor
4. **Playtest** - Full gameplay verification

## Phase 1: Pre-Test Preparation

### Build Verification
```powershell
# Clean compile
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

### Test Scope Definition
1. Identify what changed
2. List affected systems
3. Define test cases
4. Prepare test data

## Phase 2: Unit Testing

### Automation Test Setup
```cpp
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FQuestAcceptTest,
    "cplus.QuestSystem.AcceptQuest",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
)

bool FQuestAcceptTest::RunTest(const FString& Parameters)
{
    // Arrange
    UQuestSubsystem* Subsystem = /* get subsystem */;
    UQuestDefinition* Quest = /* create test quest */;
    
    // Act
    bool bResult = Subsystem->AcceptQuest(Quest);
    
    // Assert
    TestTrue(TEXT("Quest should be accepted"), bResult);
    TestTrue(TEXT("Quest should be active"), Subsystem->IsQuestActive(Quest->QuestID));
    
    return true;
}
```

### Running Automation Tests
```
1. Window → Developer Tools → Session Frontend
2. Automation tab
3. Filter by "cplus"
4. Run selected tests
```

### Console Commands
```
Automation RunTests cplus
Automation List cplus
```

## Phase 3: Integration Testing

### System Interaction Tests
| Test | Systems | Verification |
|------|---------|--------------|
| Quest Accept | QuestSubsystem + QuestTracker | Quest appears in tracker |
| NPC Interaction | QuestGiver + Interactable | Quest offered to player |
| Quest Kill | QuestTarget + KillTask | Kill count increments |
| Quest Complete | QuestSubsystem + Rewards | Rewards applied |

### Test Scenario Template
```markdown
## Test: [Scenario Name]

### Preconditions
- [Required state]

### Steps
1. [Action]
2. [Action]
3. [Action]

### Expected Results
- [Result 1]
- [Result 2]

### Actual Results
- [ ] Pass / [ ] Fail
- Notes: [observations]
```

## Phase 4: Editor Verification

### Visual Verification Points
- [ ] Actor spawns correctly
- [ ] Components visible in details
- [ ] Properties editable
- [ ] Blueprint access works
- [ ] No error messages in log

### Console Verification
```
LogQuest: Quest accepted: [QuestID]
LogQuest: Task progress: [X/Y]
LogQuest: Quest completed: [QuestID]
```

## Phase 5: Playtest

### Gameplay Test Flow
1. Start PIE (Play In Editor)
2. Execute gameplay scenario
3. Observe behavior
4. Check Output Log for errors
5. Document issues

### Test Coverage Areas
- [ ] Normal flow (happy path)
- [ ] Edge cases (empty data, max values)
- [ ] Error cases (null pointers, invalid IDs)
- [ ] Performance (many instances)
- [ ] Persistence (save/load)

## Test Documentation

### Bug Report Template
```markdown
## Bug: [Title]

### Severity
- [ ] Critical (crash/data loss)
- [ ] High (feature broken)
- [ ] Medium (workaround exists)
- [ ] Low (cosmetic)

### Steps to Reproduce
1. [Step]
2. [Step]

### Expected Behavior
[Description]

### Actual Behavior
[Description]

### Environment
- Build: Development
- Map: [MapName]
- Last working: [commit/date]
```

## Quick Validation Commands

```cpp
// In-game console commands for testing
UFUNCTION(Exec)
void DebugAcceptQuest(FName QuestID);

UFUNCTION(Exec)
void DebugCompleteQuest(FName QuestID);

UFUNCTION(Exec)
void DebugListActiveQuests();
```
