# UI System Architecture

**Verified:** 2026-01-15  
**Source:** Actual C++ implementation in `Source/cplus/UI/`

## Overview

UI-systemet är centraliserat genom `UUIManager` som hanterar ALL UI i spelet. Systemet är event-drivet och integrerar med Quest, Save och GameMode-systemen.

---

## Core Architecture

### Central Manager: UUIManager

**Location:** `Source/cplus/UI/UIManager.h/cpp`  
**Type:** `UActorComponent`  
**Attached to:** `ABasePlayerCharacter`

#### Responsibilities
- Skapar och hanterar Pause Menu
- Äger och exponerar `UQuestUIManager` (sub-manager)
- Centraliserad kontroll för all UI visibility

#### Components
```cpp
// Widget Classes (set in Blueprint)
TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

// Sub-Managers
UQuestUIManager* QuestUIManager;  // Created in constructor

// Widget Instances
TObjectPtr<UPauseMenuWidget> PauseMenuWidget;  // Created in BeginPlay
```

#### Lifecycle
1. **Constructor:** Skapar `QuestUIManager` som subobject
2. **BeginPlay:** Skapar `PauseMenuWidget` och lägger till viewport
3. **EndPlay:** Cleanup

#### Key Functions
- `TogglePauseMenu()` - Öppna/stäng pause menu
- `ShowPauseMenu()` / `HidePauseMenu()` - Explicit kontroll
- `IsPauseMenuVisible()` - Status check
- `GetQuestUIManager()` - Accessor för quest UI

---

## Pause Menu System

### UPauseMenuWidget

**Location:** `Source/cplus/UI/PauseMenuWidget.h/cpp`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_PauseMenu` (binds `MenuContainer` och `TitleText`)

#### Features
- **Auto-creates buttons** i C++ (Resume, Save, Load, Main Menu, Quit)
- **Full C++ logic** - Blueprint behöver bara binda `MenuContainer` och `TitleText`
- **Input mode switching** - UI mode när öppen, Game mode när stängd
- **Game pause integration** - Pausar spelet automatiskt

#### Widget Bindings (Blueprint)
```cpp
UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
UVerticalBox* MenuContainer;  // Container för auto-skapade knappar

UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
UTextBlock* TitleText;  // "PAUSED" text
```

#### Button Handlers
1. **Resume** → `HidePauseMenu()` - Återgår till spel
2. **Save Game** → `USaveGameSubsystem::SaveGame()` - Sparar till current slot
3. **Load Game** → `USaveGameSubsystem::LoadGame()` + reload level
4. **Main Menu** → `ABaseGameMode::ShowMainMenu()` - Visar main menu
5. **Quit** → `UKismetSystemLibrary::QuitGame()` - Avslutar spel

#### Input Mode Management
```cpp
// Show
SetVisibility(ESlateVisibility::Visible);
UGameplayStatics::SetGamePaused(GetWorld(), true);
PC->SetInputMode(FInputModeUIOnly);
PC->bShowMouseCursor = true;

// Hide
SetVisibility(ESlateVisibility::Hidden);
UGameplayStatics::SetGamePaused(GetWorld(), false);
PC->SetInputMode(FInputModeGameOnly);
PC->bShowMouseCursor = false;
```

---

## Main Menu System

### UMainMenuWidget

**Location:** `Source/cplus/UI/MainMenuWidget.h/cpp`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_MainMenu` (binds alla knappar)

#### Features
- **New Game** - Startar nytt spel i slot 0 (TODO: slot selection)
- **Continue** - Laddar senaste save (disabled om inga saves finns)
- **Load Game** - Laddar från slot 0 (TODO: slot selection)
- **Settings** - Placeholder (TODO: implementation)
- **Exit Game** - Avslutar applikation

#### Widget Bindings (Blueprint)
```cpp
UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
TObjectPtr<UTextBlock> TitleText;
TObjectPtr<UVerticalBox> ButtonContainer;
TObjectPtr<UButton> NewGameButton;
TObjectPtr<UButton> ContinueButton;
TObjectPtr<UButton> LoadGameButton;
TObjectPtr<UButton> SettingsButton;
TObjectPtr<UButton> ExitGameButton;
```

#### Button State Management
- **Continue button** disabled om inga saves finns
- Kollar alla slots (0-2) via `USaveGameSubsystem::DoesSaveExist()`

### AMainMenuActor

**Location:** `Source/cplus/UI/MainMenuActor.h/cpp`  
**Type:** `AActor`  
**Blueprint:** `BP_MainMenuActor` (placeras i level)

#### Features
- **Camera component** för Main Menu view
- **Scene root** för att organisera background elements
- **Camera switching** med blend (0.5s)
- **View target restoration** när Main Menu stängs

#### Key Functions
```cpp
void ActivateMenuCamera();    // Byter till menu camera
void DeactivateMenuCamera();  // Återställer till player camera/pawn
```

#### Camera Management
```cpp
// Activate
CachedPlayerController = GetPlayerController(0);
OriginalViewTarget = PC->GetViewTarget();  // Spara current
PC->SetViewTargetWithBlend(this, 0.5f);    // Byt till menu camera

// Deactivate
PC->SetViewTargetWithBlend(OriginalViewTarget, 0.5f);  // Återställ
```

---

## Quest UI System

### UQuestUIManager

**Location:** `Source/cplus/QuestSystem/UI/QuestUIManager.h/cpp`  
**Type:** `UActorComponent`  
**Owner:** `UUIManager` (created as subobject)

#### Responsibilities
- Hanterar quest notifications och journal
- Binder till `UQuestSubSystem` delegates
- Auto-visar notifications baserat på settings

#### Widget Classes (set in Blueprint)
```cpp
TSubclassOf<UQuestNotificationWidget> NotificationWidgetClass;
TSubclassOf<UQuestJournalWidget> JournalWidgetClass;
```

#### Settings
```cpp
bool bAutoShowQuestStarted = true;      // Auto-visa när quest startar
bool bAutoShowQuestCompleted = true;    // Auto-visa när quest slutförs
bool bAutoShowObjectiveUpdated = false; // Auto-visa vid objective update
```

#### Lifecycle
1. **BeginPlay:**
   - Hämtar `UQuestSubSystem` från GameInstance
   - Binder till delegates (`OnQuestStarted`, `OnQuestCompleted`, `OnQuestTaskUpdated`)
   - Skapar `NotificationWidget` och lägger till viewport (Z-order 100)
   - Skapar `JournalWidget` (läggs INTE till viewport förrän öppnad)

2. **Delegate Handlers:**
   - `HandleQuestStarted()` → Visar notification + auto-track
   - `HandleQuestCompleted()` → Visar notification + untrack
   - `HandleQuestTaskUpdated()` → Uppdaterar tracked quest progress

#### Key Functions
```cpp
// Notifications
void ShowQuestStartedNotification(FName QuestID, UQuestDefinition* Quest);
void ShowQuestCompletedNotification(FName QuestID, const FQuestReward& Rewards);
void ShowObjectiveUpdatedNotification(FName QuestID, int32 ObjectiveIndex, int32 TaskIndex);

// Journal
void OpenQuestJournal();   // AddToViewport + UI input mode
void CloseQuestJournal();  // RemoveFromParent + Game input mode
void ToggleQuestJournal(); // Toggle mellan open/close

// Visibility Control
void HideUI();  // Döljer notifications och journal (för Main Menu)
void ShowUI();  // Visar notifications och journal (efter Main Menu)
```

### UQuestNotificationWidget

**Location:** `Source/cplus/QuestSystem/UI/QuestNotificationWidget.h`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_QuestNotification`

#### Features (från header)
- Quest started notifications
- Quest completed notifications
- Objective updated notifications
- Quest tracking display (current objective + progress)
- Auto-track new quests (configurable)

### UQuestJournalWidget

**Location:** `Source/cplus/QuestSystem/UI/QuestJournalWidget.h`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_QuestJournal`

#### Features (från header)
- Visar alla active quests
- Quest selection och detail view
- Progress tracking per quest
- Integration med `UQuestSubSystem`

### UQuestProgressWidget

**Location:** `Source/cplus/QuestSystem/UI/QuestProgressWidget.h`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_QuestProgress`

#### Features
- Detaljerad progress för en quest
- Visar alla objectives och tasks
- Progress bars och counters
- Task type indicators (Collect, Kill, Interact, Explore, Talk)
- Color coding (green=complete, yellow=in progress, white=not started)

#### Widget Bindings
```cpp
UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
TObjectPtr<UTextBlock> QuestTitleText;
TObjectPtr<UTextBlock> QuestDescriptionText;
TObjectPtr<UVerticalBox> ObjectivesContainer;
```

### UQuestGiverWidget

**Location:** `Source/cplus/QuestSystem/UI/QuestGiverWidget.h`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_QuestGiverDialog`

#### Features
- Visar quest information från NPC
- Accept/Decline buttons
- Quest icon (optional)
- Full C++ implementation

#### Widget Bindings
```cpp
UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
TObjectPtr<UTextBlock> TitleText;
TObjectPtr<UTextBlock> DescriptionText;
TObjectPtr<UButton> AcceptButton;
TObjectPtr<UButton> DeclineButton;
TObjectPtr<UImage> QuestIcon;  // BindWidgetOptional
```

### UQuestCompletionWidget

**Location:** `Source/cplus/QuestSystem/UI/QuestCompletionWidget.h`  
**Type:** `UUserWidget`  
**Blueprint:** `WBP_QuestCompletionDialog`

#### Features
- Visar quest completion message
- Displays rewards (XP, Gold, Reputation)
- Continue button
- Quest completion icon (optional)

#### Widget Bindings
```cpp
UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
TObjectPtr<UTextBlock> CompletionTitleText;  // "Quest Completed!"
TObjectPtr<UTextBlock> QuestTitleText;
TObjectPtr<UTextBlock> CompletionMessageText;
TObjectPtr<UVerticalBox> RewardsContainer;
TObjectPtr<UTextBlock> XPRewardText;
TObjectPtr<UTextBlock> GoldRewardText;
TObjectPtr<UTextBlock> ReputationRewardText;  // BindWidgetOptional
TObjectPtr<UButton> ContinueButton;
TObjectPtr<UImage> CompletionIcon;  // BindWidgetOptional
```

---

## Input Integration

### ABasePlayerCharacter Input Bindings

**Location:** `Source/cplus/Core/BasePlayerCharacter.h/cpp`

#### Input Actions
```cpp
UPROPERTY(EditAnywhere, Category = "Input")
UInputAction* PauseAction;        // Pause menu toggle

UPROPERTY(EditAnywhere, Category = "Input")
UInputAction* OpenJournalAction;  // Quest journal toggle
```

#### Input Handlers
```cpp
// Pause Menu
void DoPauseMenu()
{
    if (UIManager)
    {
        UIManager->TogglePauseMenu();
    }
}

// Quest Journal
void DoOpenJournal()
{
    if (UIManager && UIManager->GetQuestUIManager())
    {
        UIManager->GetQuestUIManager()->ToggleQuestJournal();
    }
}
```

#### Setup
Input actions binds i `SetupPlayerInputComponent()`:
- `PauseAction` → `DoPauseMenu()`
- `OpenJournalAction` → `DoOpenJournal()`

---

## Integration Points

### 1. Save System Integration

**PauseMenuWidget:**
```cpp
// Save Game
USaveGameSubsystem* SaveSub = GameInstance->GetSubsystem<USaveGameSubsystem>();
SaveSub->SaveGame(SlotToSave);

// Load Game
SaveSub->LoadGame(SlotToLoad);
UGameplayStatics::OpenLevel(this, FName("Lvl_Denker"));
```

**MainMenuWidget:**
```cpp
// New Game
SaveSub->NewGame(0, TEXT("Player"));

// Continue
SaveSub->Continue();  // Loads most recent save

// Button State
bool bAnySaveExists = SaveSub->DoesSaveExist(i);
ContinueButton->SetIsEnabled(bAnySaveExists);
```

### 2. Quest System Integration

**QuestUIManager → QuestSubSystem:**
```cpp
// Bind to delegates
QuestSubsystem->OnQuestStarted.AddDynamic(this, &UQuestUIManager::HandleQuestStarted);
QuestSubsystem->OnQuestCompleted.AddDynamic(this, &UQuestUIManager::HandleQuestCompleted);
QuestSubsystem->OnQuestTaskUpdated.AddDynamic(this, &UQuestUIManager::HandleQuestTaskUpdated);

// Get quest data
UQuestDefinition* Quest = QuestSubsystem->GetQuestDefinition(QuestID);
int32 Progress = QuestSubsystem->GetTaskProgress(QuestID, TaskIndex);
```

### 3. GameMode Integration

**PauseMenuWidget → BaseGameMode:**
```cpp
// Return to Main Menu
ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(GameMode);
BaseGameMode->ShowMainMenu();
```

**BaseGameMode:**
```cpp
// Main Menu Control
void ShowMainMenu();  // Creates widget + activates camera
void HideMainMenu();  // Removes widget + restores camera
bool IsMainMenuActive() const;  // Status check
```

**QuestUIManager checks Main Menu state:**
```cpp
// Hide quest UI when Main Menu is active
if (BaseGameMode->IsMainMenuActive())
{
    NotificationWidget->SetVisibility(ESlateVisibility::Hidden);
}
```

---

## Widget Lifecycle Summary

### Pause Menu
1. **Created:** `UIManager::BeginPlay()` → `CreatePauseMenuWidget()`
2. **Added to Viewport:** Immediately (hidden by default)
3. **Visibility:** Toggled via `TogglePauseMenu()`
4. **Destroyed:** `UIManager::EndPlay()`

### Main Menu
1. **Created:** `ABaseGameMode::ShowMainMenu()`
2. **Added to Viewport:** When created
3. **Camera:** `AMainMenuActor::ActivateMenuCamera()`
4. **Destroyed:** `ABaseGameMode::HideMainMenu()`

### Quest Notification
1. **Created:** `QuestUIManager::BeginPlay()` → `CreateNotificationWidget()`
2. **Added to Viewport:** Immediately (Z-order 100)
3. **Visibility:** Always visible (unless Main Menu active)
4. **Destroyed:** `QuestUIManager::EndPlay()`

### Quest Journal
1. **Created:** `QuestUIManager::BeginPlay()` → `CreateJournalWidget()`
2. **Added to Viewport:** When opened via `OpenQuestJournal()`
3. **Removed from Viewport:** When closed via `CloseQuestJournal()`
4. **Destroyed:** `QuestUIManager::EndPlay()`

### Quest Giver/Completion Dialogs
1. **Created:** On-demand when NPC interaction occurs
2. **Added to Viewport:** When shown
3. **Removed:** When player clicks Accept/Decline/Continue
4. **Destroyed:** After removal

---

## Z-Order Hierarchy

```
Higher (Front)
    200 - Quest Giver/Completion Dialogs (modal)
    100 - Quest Notifications (persistent)
     50 - Quest Journal (overlay)
      0 - Pause Menu (overlay)
      0 - Main Menu (overlay)
Lower (Back)
```

---

## Blueprint Requirements

### Minimal Blueprint Setup

#### WBP_PauseMenu
**Required Bindings:**
- `MenuContainer` (UVerticalBox) - Container för auto-skapade knappar
- `TitleText` (UTextBlock) - Title text ("PAUSED")

**C++ skapar automatiskt:**
- Resume button
- Save Game button
- Load Game button
- Main Menu button
- Quit button

#### WBP_MainMenu
**Required Bindings:**
- `TitleText` (UTextBlock)
- `ButtonContainer` (UVerticalBox)
- `NewGameButton` (UButton)
- `ContinueButton` (UButton)
- `LoadGameButton` (UButton)
- `SettingsButton` (UButton)
- `ExitGameButton` (UButton)

**C++ hanterar:**
- Button click handlers
- Button state management
- Save system integration

#### WBP_QuestNotification
**Required Bindings:**
- Defined in `UQuestNotificationWidget.h` (TODO: verify)

#### WBP_QuestJournal
**Required Bindings:**
- Defined in `UQuestJournalWidget.h` (TODO: verify)

#### WBP_QuestGiverDialog
**Required Bindings:**
- `TitleText` (UTextBlock)
- `DescriptionText` (UTextBlock)
- `AcceptButton` (UButton)
- `DeclineButton` (UButton)
- `QuestIcon` (UImage) - Optional

#### WBP_QuestCompletionDialog
**Required Bindings:**
- `CompletionTitleText` (UTextBlock)
- `QuestTitleText` (UTextBlock)
- `CompletionMessageText` (UTextBlock)
- `RewardsContainer` (UVerticalBox)
- `XPRewardText` (UTextBlock)
- `GoldRewardText` (UTextBlock)
- `ReputationRewardText` (UTextBlock) - Optional
- `ContinueButton` (UButton)
- `CompletionIcon` (UImage) - Optional

---

## Key Design Principles

### 1. Centralized Management
- **All UI** går genom `UUIManager`
- **Quest UI** delegeras till `UQuestUIManager` sub-manager
- **Single source of truth** för UI state

### 2. Event-Driven
- Quest UI binder till `UQuestSubSystem` delegates
- Automatic notifications vid quest events
- Decoupled från game logic

### 3. C++ Logic, Blueprint Styling
- **All business logic** i C++
- **Blueprint** används bara för:
  - Widget bindings
  - Visual styling
  - Layout

### 4. Input Mode Management
- **Game mode** - Normal gameplay
- **UI mode** - Menus och dialogs
- **Automatic switching** när UI öppnas/stängs

### 5. Lifecycle Control
- Widgets skapas i BeginPlay
- Persistent widgets (notifications) läggs till viewport direkt
- Modal widgets (journal, dialogs) läggs till on-demand

---

## TODO / Future Improvements

### From Code Comments

1. **Quest Dialog Refactoring** (`BasePlayerCharacter.h:163-164`)
   - Move `QuestGiverWidgetClass` och `QuestCompletionWidgetClass` till `QuestUIManager`
   - All quest UI bör hanteras av `QuestUIManager`, inte direkt på player

2. **Save Slot Selection** (`MainMenuWidget.cpp`)
   - Implement `ShowNewGameSlotSelection()`
   - Implement `ShowLoadGameSlotSelection()`
   - Currently hardcoded to slot 0

3. **Settings Menu** (`MainMenuWidget.cpp:197-202`)
   - Implement settings menu functionality
   - Currently placeholder

4. **Inventory UI** (Not implemented)
   - Create `UInventoryUIManager` sub-manager
   - Inventory widgets och integration

---

## Verification Status

✅ **Verified against source code:**
- `UIManager.h/cpp`
- `PauseMenuWidget.h/cpp`
- `MainMenuWidget.h/cpp`
- `MainMenuActor.h/cpp`
- `QuestUIManager.h/cpp`
- `QuestProgressWidget.h`
- `QuestGiverWidget.h`
- `QuestCompletionWidget.h`
- `BasePlayerCharacter.h/cpp`
- `BaseGameMode.h`

⚠️ **Not fully verified:**
- `QuestNotificationWidget` implementation details
- `QuestJournalWidget` implementation details
- Blueprint widget implementations

**Last Updated:** 2026-01-15
