# Quest Progress Visualization Guide

> Complete guide for implementing quest progress tracking UI

## Overview

The Quest Progress Visualization System provides complete progress tracking for all quest types:
- **Collect** - Track item collection (3/10 Gold Coins)
- **Kill** - Track enemy kills (5/20 Bandits)
- **Interact** - Track object interactions (2/5 Levers)
- **Explore** - Track location discovery (1/3 Locations)
- **Talk** - Track NPC conversations (0/1 Merchant)

---

## Components

### 1. QuestProgressWidget (C++)
Main widget for displaying complete quest progress.

**Features:**
- Quest title and description
- All objectives with tasks
- Progress bars and counters
- Task type indicators
- Color-coded completion status

**Blueprint Setup:**
```
WBP_QuestProgress (Blueprint Widget)
├── QuestTitleText (TextBlock) - Bound
├── QuestDescriptionText (TextBlock) - Bound
└── ObjectivesContainer (VerticalBox) - Bound
    └── [Dynamically populated with task entries]
```

**Usage in C++:**
```cpp
UQuestProgressWidget* ProgressWidget = CreateWidget<UQuestProgressWidget>(PC, ProgressWidgetClass);
ProgressWidget->SetQuest(QuestDefinition, CurrentObjectiveIndex);
ProgressWidget->AddToViewport();
```

**Usage in Blueprint:**
```
Set Quest (Quest Definition, Current Objective Index)
↓
Widget automatically updates with:
- Quest title
- Quest description
- All objectives and tasks
```

---

### 2. QuestTaskEntryWidget (C++)
Individual task display widget.

**Features:**
- Task type icon/text (Collect, Kill, etc.)
- Task description
- Progress counter (3/10)
- Progress bar (visual)
- Color-coded status

**Blueprint Setup:**
```
WBP_QuestTaskEntry (Blueprint Widget)
├── TaskTypeText (TextBlock) - Optional
├── TaskDescriptionText (TextBlock) - Optional
├── ProgressCounterText (TextBlock) - Optional
├── ProgressBar (ProgressBar) - Optional
└── TaskTypeIcon (Image) - Optional
```

**Usage in C++:**
```cpp
UQuestTaskEntryWidget* TaskWidget = CreateWidget<UQuestTaskEntryWidget>(PC, TaskEntryClass);
TaskWidget->SetTask(QuestTask);
TaskWidget->AddToViewport();
```

**Usage in Blueprint:**
```
Set Task (Quest Task)
↓
Widget automatically updates with:
- Task type (Collect/Kill/Interact/Explore/Talk)
- Description
- Progress (3/10)
- Progress bar (30%)
- Color (Green/Yellow/Gray)
```

---

## Task Types

### Quest Task: Interact (or Collect)

**Actor:** `AQuestInteractableObject`

**Supports TWO quest task types:**
- **Collect tasks:** Uses `ItemID` (no tag needed)
- **Interact tasks:** Uses `InteractionTag`

**Setup in Editor:**
1. Drag `AQuestInteractableObject` into level
2. **For Collect tasks:**
   - Set `ItemID` = `Item.QuestItem` (or your item ID)
   - Leave `InteractionTag` empty
3. **For Interact tasks:**
   - Set `InteractionTag` = `Quest.Interact.Lever` (must match quest definition)
   - Leave `ItemID` empty
4. Configure visual mesh in `MeshComponent`
5. Adjust `InteractionSphere` radius (default: 200 units)
6. Set `InteractionPrompt` text (default: "Press E to interact")
7. Optional: Set `bCanInteractMultipleTimes` if reusable
8. Optional: Add `InteractEffect` (particle system)
9. Optional: Add `InteractSound` (sound effect)

**Quest Definition Setup:**
```
Quest: "Exploration Time"
└── Objective 1: "Interact with the quest object"
    └── Task: Quest Task Interact
        ├── Type: Interact
        ├── Target Tag: Quest.Interact.Lever
        ├── Required Count: 1
        └── Description: "Interact with objects (0/1)"
```

**How It Works:**
1. Player walks near object (within InteractionSphere)
2. `BasePlayerCharacter` detects `IQuestInteractable` interface
3. Player presses E to interact
4. `AQuestInteractableObject::Interact_Implementation()` called
5. `QuestTargetComponent->OnInteracted()` notifies quest system
6. Quest progress updates automatically
7. Visual/audio effects play (if configured)

**Example Tags:**
- `Quest.Interact.Lever` - Lever/switch
- `Quest.Interact.Chest` - Chest/container
- `Quest.Interact.Door` - Door/gate
- `Quest.Interact.Terminal` - Computer/terminal
- `Quest.Interact.Statue` - Statue/monument

---

### All Task Types Support:

| Task Type | Description | Example |
|-----------|-------------|---------|
| **Collect** | Collect items | "Collect: 3/10 Gold Coins" |
| **Kill** | Kill enemies | "Kill: 5/20 Bandits" |
| **Interact** | Interact with objects | "Interact: 2/5 Levers" |
| **Explore** | Reach locations | "Explore: 1/3 Ancient Ruins" |
| **Talk** | Talk to NPCs | "Talk: 0/1 Merchant" |

---

## API Reference

### UQuestTask (Base Class)

#### GetProgressText(bool bShowPercentage)
Returns formatted progress text.
```cpp
// Fraction format
FText Progress = Task->GetProgressText(false); // "3/10"

// Percentage format
FText Progress = Task->GetProgressText(true);  // "30%"
```

#### GetProgressPercent()
Returns progress as float (0.0 - 1.0).
```cpp
float Percent = Task->GetProgressPercent(); // 0.3
```

#### GetTaskTypeName()
Returns task type name.
```cpp
FText Type = Task->GetTaskTypeName(); // "Collect", "Kill", etc.
```

#### GetDetailedProgressText()
Returns complete progress description.
```cpp
FText Details = Task->GetDetailedProgressText();
// "Collect: 3/10 - Collect Gold Coins"
```

---

## Blueprint Implementation Example

### Creating Quest Progress Display

1. **Create WBP_QuestProgress Widget:**
```
Canvas Panel
└── Vertical Box (ObjectivesContainer)
    ├── Text Block (QuestTitleText)
    ├── Text Block (QuestDescriptionText)
    └── Scroll Box
        └── [Task entries added dynamically]
```

2. **Bind Widget Variables:**
- Set `QuestTitleText` to bind to TextBlock
- Set `QuestDescriptionText` to bind to TextBlock
- Set `ObjectivesContainer` to bind to VerticalBox

3. **Override OnQuestSet Event:**
```blueprint
Event OnQuestSet
├── Get Quest Objectives
├── For Each Objective
│   ├── Create WBP_QuestTaskEntry
│   ├── Set Task
│   └── Add to ObjectivesContainer
```

4. **Override OnProgressRefreshed Event:**
```blueprint
Event OnProgressRefreshed
├── Clear ObjectivesContainer
├── Rebuild task entries
└── Update progress bars
```

---

### Creating Task Entry Widget

1. **Create WBP_QuestTaskEntry Widget:**
```
Horizontal Box
├── Text Block (TaskTypeText) - "Collect"
├── Progress Bar (ProgressBar) - Visual progress
└── Text Block (ProgressCounterText) - "3/10"
```

2. **Override OnTaskSet Event:**
```blueprint
Event OnTaskSet
├── Get Task Type Name → Set TaskTypeText
├── Get Progress Text → Set ProgressCounterText
├── Get Progress Percent → Set ProgressBar
└── Get Task Color → Set ProgressBar Color
```

3. **Override OnTaskRefreshed Event:**
```blueprint
Event OnTaskRefreshed
├── Update ProgressCounterText
├── Update ProgressBar Percent
└── Update ProgressBar Color
```

---

## Integration with QuestJournalWidget

### Update Journal to Show Progress

```cpp
// In QuestJournalWidget::RefreshJournal()
void UQuestJournalWidget::RefreshJournal()
{
    // Get active quests from subsystem
    TArray<FActiveQuest> ActiveQuests = QuestSubsystem->GetActiveQuests();
    
    // For each quest, create progress widget
    for (const FActiveQuest& Quest : ActiveQuests)
    {
        UQuestProgressWidget* ProgressWidget = CreateWidget<UQuestProgressWidget>(
            this, QuestProgressWidgetClass);
        
        ProgressWidget->SetQuest(Quest.QuestDefinition, Quest.CurrentObjectiveIndex);
        QuestListContainer->AddChild(ProgressWidget);
    }
}
```

---

## Color Coding

### Task Status Colors

| Status | Color | RGB | Usage |
|--------|-------|-----|-------|
| **Complete** | Green | (0, 255, 0) | Task finished |
| **In Progress** | Yellow | (255, 255, 0) | Task started |
| **Not Started** | Gray | (128, 128, 128) | Task pending |

### Implementation
```cpp
FLinearColor GetTaskColor(UQuestTask* Task)
{
    if (Task->IsComplete())
        return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green
    else if (Task->CurrentCount > 0)
        return FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    else
        return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray
}
```

---

## Real-Time Updates

### Automatic Progress Updates

The system automatically updates when:
1. **Task progress changes** - `OnQuestTaskUpdated` delegate fires
2. **Quest completes** - `OnQuestCompleted` delegate fires
3. **Quest starts** - `OnQuestStarted` delegate fires

### Manual Refresh
```cpp
// Refresh progress display
ProgressWidget->RefreshProgress();

// Refresh single task
TaskWidget->RefreshTask();
```

---

## Examples

### Example 1: Collect Quest
```
Quest: Gather Resources
├── Objective 1: Collect Materials
│   ├── Collect: 3/10 Wood [████░░░░░░] 30%
│   ├── Collect: 5/5 Stone [██████████] 100% ✓
│   └── Collect: 0/3 Iron [░░░░░░░░░░] 0%
```

### Example 2: Kill Quest
```
Quest: Clear the Camp
├── Objective 1: Eliminate Bandits
│   ├── Kill: 8/10 Bandits [████████░░] 80%
│   └── Kill: 0/1 Bandit Leader [░░░░░░░░░░] 0%
```

### Example 3: Mixed Quest
```
Quest: Help the Village
├── Objective 1: Gather Information
│   ├── Talk: 1/1 Village Elder [██████████] 100% ✓
│   └── Explore: 1/3 Locations [███░░░░░░░] 33%
├── Objective 2: Collect Evidence
│   ├── Collect: 0/5 Clues [░░░░░░░░░░] 0%
│   └── Interact: 0/3 Objects [░░░░░░░░░░] 0%
```

---

## Testing

### Test Each Task Type

1. **Collect Task:**
```cpp
UQuestTask_Collect* CollectTask = NewObject<UQuestTask_Collect>();
CollectTask->TaskDescription = FText::FromString("Collect Gold Coins");
CollectTask->TargetCount = 10;
CollectTask->CurrentCount = 3;
// Should display: "Collect: 3/10 - Collect Gold Coins"
```

2. **Kill Task:**
```cpp
UQuestTask_Kill* KillTask = NewObject<UQuestTask_Kill>();
KillTask->TaskDescription = FText::FromString("Kill Bandits");
KillTask->TargetCount = 20;
KillTask->CurrentCount = 5;
// Should display: "Kill: 5/20 - Kill Bandits"
```

3. **Talk Task:**
```cpp
UQuestTask_Talk* TalkTask = NewObject<UQuestTask_Talk>();
TalkTask->TaskDescription = FText::FromString("Talk to Merchant");
TalkTask->TargetCount = 1;
TalkTask->CurrentCount = 0;
// Should display: "Talk: 0/1 - Talk to Merchant"
```

---

## Troubleshooting

### Widget Not Updating
**Problem:** Progress doesn't update when task completes.
**Solution:** Ensure delegates are bound in QuestUIManager:
```cpp
QuestSubsystem->OnQuestTaskUpdated.AddDynamic(this, &UQuestUIManager::HandleTaskUpdated);
```

### Wrong Progress Text
**Problem:** Shows "Task: 0/0" instead of proper values.
**Solution:** Check that TargetCount is set in QuestTask definition.

### Colors Not Showing
**Problem:** All tasks show same color.
**Solution:** Ensure ProgressBar color is set in RefreshTask():
```cpp
ProgressBar->SetFillColorAndOpacity(GetTaskColor(Task));
```

---

## Best Practices

1. **Always use GetProgressText()** - Don't format manually
2. **Cache widget references** - Don't recreate every frame
3. **Use delegates for updates** - Don't poll for changes
4. **Test all task types** - Ensure each works correctly
5. **Color-code status** - Visual feedback is important
6. **Show task type icons** - Help players understand objectives

---

**Version:** 1.0  
**Last Updated:** 2026-01-14  
**Project:** cplus (Unreal Engine 5.7)
