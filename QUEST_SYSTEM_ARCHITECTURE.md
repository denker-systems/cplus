# Quest System Architecture

## 📋 Översikt

Ett modulärt, data-drivet quest system för Unreal Engine 5.7 byggt med component-based design och interfaces för maximal flexibilitet.

---

## 🎯 Design Principer

- ✅ **Data-Driven** - Quests definieras i DataAssets, inte hårdkodade
- ✅ **Component-Based** - Använd Components istället för Actor-hierarkier
- ✅ **Interface-Driven** - Interfaces för quest interactions
- ✅ **Event-Based** - Observer pattern med Unreal Delegates
- ✅ **Modular** - Nya task-typer läggs till genom inheritance
- ✅ **Saveable** - Quest progress kan serialiseras
- ✅ **Blueprint-Friendly** - Designers kan skapa quests utan C++

---

## 🏗️ Arkitektur Översikt

```
┌─────────────────────────────────────────────────────────────┐
│                    UQuestSubsystem                          │
│         (GameInstanceSubsystem - Global Manager)            │
│  • Quest Library                                            │
│  • Active Quests Tracking                                   │
│  • Event Broadcasting                                       │
│  • Save/Load State                                          │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ Manages
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  UQuestDefinition                           │
│              (PrimaryDataAsset - Quest Data)                │
│  • Quest ID, Title, Description                             │
│  • Array<FQuestObjective>                                   │
│  • Prerequisites & Rewards                                  │
│  • Gameplay Tags                                            │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ Contains
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  FQuestObjective                            │
│                  (Struct - Objective Data)                  │
│  • Array<UQuestTask*>                                       │
│  • Completion Rules (All/Any/N of M)                        │
│  • Sequential vs Parallel                                   │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ Contains
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    UQuestTask                               │
│              (UObject - Abstract Base Class)                │
│  • Task ID, Description                                     │
│  • Progress (Current/Target)                                │
│  • Virtual: OnStarted(), UpdateProgress(), IsComplete()     │
│                                                             │
│  Subclasses:                                                │
│  ├─ UQuestTask_Kill                                         │
│  ├─ UQuestTask_Collect                                      │
│  ├─ UQuestTask_Interact                                     │
│  ├─ UQuestTask_Location                                     │
│  ├─ UQuestTask_Escort                                       │
│  ├─ UQuestTask_Timer                                        │
│  └─ UQuestTask_Custom (Blueprint-implementable)             │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔧 Component System

### UQuestTrackerComponent
**Syfte:** Trackar quest progress för Player/AI som HAR quests

**Används på:**
- `AShooterCharacter` (Player)
- AI Companions

**Funktionalitet:**
```cpp
class UQuestTrackerComponent : public UActorComponent
{
    // Active quests
    TArray<FActiveQuest> ActiveQuests;
    TArray<FName> CompletedQuests;
    
    // API
    void AcceptQuest(UQuestDefinition* Quest);
    void AbandonQuest(FName QuestID);
    void CompleteQuest(FName QuestID);
    
    // Delegates
    FOnQuestAccepted OnQuestAccepted;
    FOnQuestProgressed OnQuestProgressed;
    FOnQuestCompleted OnQuestCompleted;
};
```

---

### UQuestGiverComponent
**Syfte:** NPCs som GER quests till spelare

**Används på:**
- `AShooterNPC` (Quest NPCs)
- Interactable objects (quest boards, etc)

**Funktionalitet:**
```cpp
class UQuestGiverComponent : public UActorComponent
{
    // Available quests
    TArray<UQuestDefinition*> AvailableQuests;
    
    // API
    bool CanOfferQuest(UQuestDefinition* Quest, AActor* Player);
    TArray<UQuestDefinition*> GetAvailableQuests(AActor* Player);
    void OfferQuest(UQuestDefinition* Quest, AActor* Player);
    
    // Delegates
    FOnQuestOffered OnQuestOffered;
    FOnQuestTurnedIn OnQuestTurnedIn;
};
```

---

### UQuestTargetComponent
**Syfte:** Actors som är TARGETS i quests (döda, samla, interagera)

**Används på:**
- `AShooterNPC` (Enemies, NPCs)
- Items (Collectibles, Interactables)
- Triggers (Locations, Volumes)

**Funktionalitet:**
```cpp
class UQuestTargetComponent : public UActorComponent
{
    // Quest identification
    FGameplayTagContainer QuestTags;
    FName TargetID;
    
    // Events
    void OnInteracted(AActor* Interactor);
    void OnKilled(AActor* Killer);
    void OnCollected(AActor* Collector);
    void OnDestroyed();
    
    // Notifies QuestSubsystem
    void NotifyQuestSystem(EQuestEventType EventType, AActor* Instigator);
};
```

---

## 🔌 Interface System

### IQuestInteractable
**Syfte:** Actors som kan interageras med för quests

```cpp
class IQuestInteractable
{
    virtual void Interact(AActor* Interactor) = 0;
    virtual bool CanInteract(AActor* Interactor) const = 0;
    virtual FText GetInteractionText() const = 0;
};
```

**Implementeras av:**
- NPCs (dialog)
- Doors, Levers, Buttons
- Quest items

---

### IQuestKillable
**Syfte:** Actors som kan dödas för quests

```cpp
class IQuestKillable
{
    virtual void OnKilledForQuest(AActor* Killer) = 0;
    virtual FGameplayTagContainer GetKillTags() const = 0;
};
```

**Implementeras av:**
- `AShooterNPC` (Enemies)
- Destructible objects

---

### IQuestCollectable
**Syfte:** Items som kan samlas för quests

```cpp
class IQuestCollectable
{
    virtual void OnCollectedForQuest(AActor* Collector) = 0;
    virtual FName GetItemID() const = 0;
    virtual int32 GetItemCount() const = 0;
};
```

**Implementeras av:**
- Pickup items
- Collectibles

---

### IQuestDialogProvider
**Syfte:** NPCs som kan ha dialog

```cpp
class IQuestDialogProvider
{
    virtual void StartDialog(AActor* Initiator) = 0;
    virtual TArray<FDialogOption> GetDialogOptions() const = 0;
    virtual void OnDialogChoice(int32 ChoiceIndex) = 0;
};
```

**Implementeras av:**
- Quest NPCs
- Story NPCs

---

## 🎮 Integration med Befintliga Actors

### AShooterCharacter (Player)
```cpp
class AShooterCharacter : public AcplusCharacter
{
    // BEFINTLIG KOD
    // ... weapons, health, input ...
    
    // NYTT - Quest tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
    UQuestTrackerComponent* QuestTracker;
};
```

---

### AShooterNPC (NPCs)
```cpp
class AShooterNPC : public AcplusCharacter, 
                    public IQuestInteractable,
                    public IQuestKillable
{
    // BEFINTLIG KOD
    // ... AI, weapons, health ...
    
    // NYTT - Quest functionality
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
    UQuestGiverComponent* QuestGiver;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
    UQuestTargetComponent* QuestTarget;
    
    // Interface implementations
    virtual void Interact(AActor* Interactor) override;
    virtual void OnKilledForQuest(AActor* Killer) override;
};
```

**Integration i befintlig kod:**
```cpp
void AShooterNPC::Die()
{
    // BEFINTLIG death-logik
    // ... ragdoll, effects, etc ...
    
    // NYTT - Notifiera quest system
    if (QuestTarget)
    {
        QuestTarget->OnKilled(EventInstigator);
    }
}
```

---

## 📁 Mappstruktur

```
Source/cplus/
├─ QuestSystem/
│  ├─ Core/
│  │  ├─ QuestDefinition.h/cpp         (DataAsset för quest data)
│  │  ├─ QuestTask.h/cpp               (Base class för tasks)
│  │  ├─ QuestObjective.h/cpp          (Struct för objectives)
│  │  └─ QuestSubsystem.h/cpp          (Global quest manager)
│  │
│  ├─ Components/
│  │  ├─ QuestTrackerComponent.h/cpp   (Player quest tracking)
│  │  ├─ QuestGiverComponent.h/cpp     (NPC quest giving)
│  │  └─ QuestTargetComponent.h/cpp    (Quest target marking)
│  │
│  ├─ Tasks/
│  │  ├─ QuestTask_Kill.h/cpp          (Kill X enemies)
│  │  ├─ QuestTask_Collect.h/cpp       (Collect X items)
│  │  ├─ QuestTask_Interact.h/cpp      (Interact with N objects)
│  │  ├─ QuestTask_Location.h/cpp      (Reach location)
│  │  ├─ QuestTask_Escort.h/cpp        (Protect NPC)
│  │  └─ QuestTask_Timer.h/cpp         (Survive X seconds)
│  │
│  └─ Interfaces/
│     ├─ QuestInteractable.h/cpp       (Interaction interface)
│     ├─ QuestKillable.h/cpp           (Kill target interface)
│     ├─ QuestCollectable.h/cpp        (Collectible interface)
│     └─ QuestDialogProvider.h/cpp     (Dialog interface)
│
├─ Variant_Shooter/
│  ├─ ShooterCharacter.h/cpp           (+ QuestTrackerComponent)
│  └─ AI/
│     └─ ShooterNPC.h/cpp              (+ QuestGiver/TargetComponent)
```

---

## 🔄 Workflow

### 1. Designer skapar Quest
```
1. Skapa UQuestDefinition DataAsset i Content Browser
2. Definiera Quest metadata (Title, Description, Tags)
3. Lägg till Objectives
4. Lägg till Tasks per Objective
5. Konfigurera Prerequisites & Rewards
```

### 2. Designer placerar Quest NPCs
```
1. Placera AShooterNPC i level
2. Lägg till UQuestGiverComponent
3. Tilldela Quest DataAssets till AvailableQuests
4. Konfigurera quest availability conditions
```

### 3. Designer taggar Quest Targets
```
1. Placera enemies/items i level
2. Lägg till UQuestTargetComponent
3. Tagga med Gameplay Tags (Enemy.Bandit, Item.Key, etc)
4. Konfigurera quest-specific behavior
```

### 4. Runtime Flow
```
1. Player interagerar med NPC (IQuestInteractable)
2. QuestGiverComponent erbjuder tillgängliga quests
3. Player accepterar quest via UI
4. QuestTrackerComponent börjar tracka quest
5. Player utför actions (kill, collect, interact)
6. UQuestTargetComponent notifierar QuestSubsystem
7. QuestSubsystem uppdaterar relevanta tasks
8. QuestTrackerComponent broadcastar progress events
9. UI uppdateras via delegates
10. Quest completas → Rewards ges
```

---

## 📊 Data Flow

```
Player Action (Kill Enemy)
    │
    ▼
AShooterNPC::Die()
    │
    ▼
UQuestTargetComponent::OnKilled()
    │
    ▼
UQuestSubsystem::NotifyQuestEvent()
    │
    ▼
UQuestSubsystem::UpdateRelevantTasks()
    │
    ▼
UQuestTask_Kill::UpdateProgress()
    │
    ▼
UQuestTrackerComponent::OnTaskProgressed()
    │
    ▼
Broadcast OnQuestProgressed Delegate
    │
    ▼
UI Updates (Quest Log, HUD)
```

---

## 🎯 Quest Task Types

### UQuestTask_Kill
**Syfte:** Döda X antal enemies med specifika tags

**Exempel:**
- "Döda 5 Bandits"
- "Eliminera Boss"
- "Förstör 3 Turrets"

**Data:**
```cpp
FGameplayTagContainer TargetTags;  // Enemy.Bandit
int32 TargetCount;                 // 5
int32 CurrentCount;                // 0-5
```

---

### UQuestTask_Collect
**Syfte:** Samla X antal items

**Exempel:**
- "Samla 10 Healing Potions"
- "Hitta 3 Key Fragments"

**Data:**
```cpp
FName ItemID;                      // Item.Potion.Healing
int32 TargetCount;                 // 10
int32 CurrentCount;                // 0-10
```

---

### UQuestTask_Interact
**Syfte:** Interagera med N objekt

**Exempel:**
- "Prata med 3 Villagers"
- "Aktivera 4 Switches"
- "Öppna Ancient Door"

**Data:**
```cpp
FGameplayTagContainer TargetTags;  // NPC.Villager
int32 TargetCount;                 // 3
TArray<AActor*> InteractedActors; // Track unique actors
```

---

### UQuestTask_Location
**Syfte:** Nå en specifik plats

**Exempel:**
- "Nå Mountain Peak"
- "Enter the Cave"

**Data:**
```cpp
FVector TargetLocation;            // World position
float AcceptanceRadius;            // 500.0f
bool bRequireTriggerVolume;        // true/false
```

---

### UQuestTask_Escort
**Syfte:** Skydda NPC från punkt A till B

**Exempel:**
- "Eskort Merchant to Town"
- "Protect Scientist"

**Data:**
```cpp
AActor* ProtectedNPC;              // NPC to protect
FVector Destination;               // End location
bool bNPCMustSurvive;              // true
```

---

### UQuestTask_Timer
**Syfte:** Överlev/vänta X sekunder

**Exempel:**
- "Survive 60 seconds"
- "Defend position for 2 minutes"

**Data:**
```cpp
float Duration;                    // 60.0f
float ElapsedTime;                 // 0-60
bool bFailOnDeath;                 // true
```

---

## 🎨 Example Quest Definition (JSON/DataTable)

```json
{
  "QuestID": "Q_BanditCamp",
  "Title": "Clear the Bandit Camp",
  "Description": "The bandits have been terrorizing the village. Clear their camp.",
  "QuestTags": ["Quest.Main", "Quest.Combat"],
  
  "Prerequisites": {
    "MinLevel": 5,
    "RequiredQuests": ["Q_MeetVillageElder"]
  },
  
  "Objectives": [
    {
      "Description": "Eliminate the bandits",
      "CompletionRule": "All",
      "Tasks": [
        {
          "Type": "Kill",
          "TargetTags": ["Enemy.Bandit"],
          "TargetCount": 10,
          "Description": "Kill 10 Bandits"
        },
        {
          "Type": "Kill",
          "TargetTags": ["Enemy.BanditLeader"],
          "TargetCount": 1,
          "Description": "Kill the Bandit Leader"
        }
      ]
    },
    {
      "Description": "Return to the village",
      "CompletionRule": "All",
      "Tasks": [
        {
          "Type": "Interact",
          "TargetTags": ["NPC.VillageElder"],
          "TargetCount": 1,
          "Description": "Report to Village Elder"
        }
      ]
    }
  ],
  
  "Rewards": {
    "XP": 500,
    "Gold": 100,
    "Items": ["Item.Weapon.SteelSword"]
  }
}
```

---

## 🔐 Save System Integration

### Quest Progress Serialization
```cpp
USTRUCT()
struct FQuestSaveData
{
    UPROPERTY()
    FName QuestID;
    
    UPROPERTY()
    EQuestState State;  // NotStarted, Active, Completed, Failed
    
    UPROPERTY()
    int32 CurrentObjectiveIndex;
    
    UPROPERTY()
    TMap<FName, int32> TaskProgress;  // TaskID -> Progress
    
    UPROPERTY()
    FDateTime AcceptedTime;
    
    UPROPERTY()
    FDateTime CompletedTime;
};
```

---

## 🎯 Gameplay Tags Structure

```
Quest
├─ Quest.Main          (Main story quests)
├─ Quest.Side          (Side quests)
├─ Quest.Daily         (Daily quests)
└─ Quest.Combat        (Combat-focused)

Enemy
├─ Enemy.Bandit
├─ Enemy.BanditLeader
├─ Enemy.Wildlife
└─ Enemy.Boss

NPC
├─ NPC.Villager
├─ NPC.Merchant
├─ NPC.QuestGiver
└─ NPC.Companion

Item
├─ Item.Key
├─ Item.Potion
└─ Item.QuestItem

Interaction
├─ Interaction.Door
├─ Interaction.Lever
└─ Interaction.Chest
```

---

## 🚀 Implementation Order

### Phase 1: Core System
1. ✅ Create architecture.md
2. ⏳ UQuestDefinition (DataAsset)
3. ⏳ FQuestObjective (Struct)
4. ⏳ UQuestTask (Base class)
5. ⏳ UQuestSubsystem (GameInstanceSubsystem)

### Phase 2: Components
6. ⏳ UQuestTrackerComponent
7. ⏳ UQuestGiverComponent
8. ⏳ UQuestTargetComponent

### Phase 3: Interfaces
9. ⏳ IQuestInteractable
10. ⏳ IQuestKillable
11. ⏳ IQuestCollectable

### Phase 4: Task Types
12. ⏳ UQuestTask_Kill
13. ⏳ UQuestTask_Collect
14. ⏳ UQuestTask_Interact
15. ⏳ UQuestTask_Location

### Phase 5: Integration
16. ⏳ Integrate with AShooterCharacter
17. ⏳ Integrate with AShooterNPC
18. ⏳ Create example quest
19. ⏳ Test & iterate

---

## 📝 Notes

- All quest data is **data-driven** via DataAssets
- Components are **modular** and can be added to any actor
- Interfaces provide **flexibility** for different actor types
- **Gameplay Tags** enable powerful filtering and categorization
- System is **Blueprint-friendly** for designers
- **Event-driven** architecture keeps code decoupled
- **Saveable** quest progress for persistence

---

## 🔗 Dependencies

### Build.cs Modules
```csharp
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "CoreUObject",
    "Engine",
    "GameplayTags",  // For quest categorization
    "AIModule"       // For NPC integration
});
```

---

## 📚 References

- [SUQS - Steve's Unreal Quest System](https://github.com/sinbad/SUQS)
- [Unreal Engine Gameplay Tags](https://docs.unrealengine.com/en-US/gameplay-tags-in-unreal-engine/)
- [Component-Based Design](https://docs.unrealengine.com/en-US/components-in-unreal-engine/)
- [Game Programming Patterns - Observer](http://gameprogrammingpatterns.com/observer.html)

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Author:** Quest System Architecture Team
