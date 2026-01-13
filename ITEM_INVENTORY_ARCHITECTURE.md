# Item & Inventory System Architecture

## 📋 Overview

A scalable, data-driven item and inventory system for Unreal Engine 5.7 that integrates seamlessly with the Quest System. Built with component-based design and DataAssets for maximum flexibility.

---

## 🎯 Design Principles

- ✅ **Data-Driven** - Items defined in DataAssets, not hardcoded
- ✅ **Component-Based** - Modular components for inventory, pickup, interaction
- ✅ **Quest-Integrated** - Automatic integration with Quest System via GameplayTags
- ✅ **Scalable** - Easy to add new items without code changes
- ✅ **Blueprint-Friendly** - Designers can create items and configure behavior
- ✅ **Type-Safe** - Strong typing with enums and structs

---

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    DATA LAYER (DataAssets)                   │
│  • UItemDefinition (items: weapons, potions, keys, etc.)     │
│  • UInteractableDefinition (doors, chests, levers, etc.)     │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ References
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                 RUNTIME DATA (Structs)                       │
│  • FInventoryItem (item instance with quantity, metadata)    │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ Managed by
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  COMPONENT LAYER (Logic)                     │
│  • UInventoryComponent (storage + management)                │
│  • UPickupComponent (world items → inventory)                │
│  • UInteractionComponent (use items, unlock doors)           │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ Attached to
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                   ACTOR LAYER (World)                        │
│  • AWorldItem (pickups in world)                            │
│  • AInteractableObject (doors, chests, levers)              │
│  • AShooterCharacter (player with inventory)                │
└─────────────────────────────────────────────────────────────┘
```

---

## 📦 Core Classes

### 1. UItemDefinition (DataAsset)
**Purpose:** Defines all properties of an item (data only, no logic)

**Properties:**
- Identity: ItemID, ItemType, ItemRarity
- UI: ItemName, ItemDescription, ItemIcon, RarityColor
- Gameplay Tags: ItemTags (for categorization and quest matching)
- Inventory: MaxStackSize, ItemWeight, SellValue, bCanBeDropped
- Quest: bIsQuestItem, RelatedQuests, bConsumedOnQuestComplete
- Usage: bIsUsable, bConsumeOnUse
- Visual: WorldMesh, InventoryMesh, WorldMeshScale
- Effects: HealthRestore, StaminaRestore (for consumables)

**Examples:**
- DA_Item_HealthPotion
- DA_Item_GoldenKey
- DA_Item_AmmoPack
- DA_Item_QuestDocument

---

### 2. FInventoryItem (Struct)
**Purpose:** Runtime instance of an item in inventory

**Properties:**
- ItemData: Reference to UItemDefinition
- Quantity: Stack count
- InstanceID: Unique GUID for tracking
- CustomData: TMap for item-specific data (durability, etc.)
- AcquiredTime: When item was obtained

**Functions:**
- CanStackWith(): Check if can stack with another item
- GetDisplayName(): Get formatted name with quantity

---

### 3. UInventoryComponent (Component)
**Purpose:** Manages item storage and inventory operations

**Attached to:** AShooterCharacter, AI companions

**Properties:**
- MaxSlots: Maximum number of inventory slots
- MaxWeight: Weight limit (optional)
- Items: TArray<FInventoryItem> - actual inventory storage

**API:**
- AddItem(): Add item to inventory
- RemoveItem(): Remove item from inventory
- HasItem(): Check if has item
- GetItemQuantity(): Get count of specific item
- UseItem(): Use/consume item
- DropItem(): Drop item to world
- HasQuestItems(): Check if has items for quest
- ConsumeQuestItems(): Remove items for quest completion

**Events:**
- OnInventoryChanged: Broadcast when inventory changes
- OnInventoryFull: Broadcast when trying to add to full inventory
- OnItemUsed: Broadcast when item is used

---

### 4. UPickupComponent (Component)
**Purpose:** Handles picking up items from world to inventory

**Attached to:** AWorldItem actors

**Properties:**
- ItemData: Reference to UItemDefinition
- ItemQuantity: How many items in this pickup
- bAutoPickup: Pickup on overlap or require interaction
- PickupRadius: Overlap detection radius
- QuestTarget: Reference to UQuestTargetComponent

**API:**
- PickupItem(): Attempt to pickup item
- CanBePickedUp(): Check if can be picked up

**Events:**
- OnItemPickedUp: Broadcast when item is picked up

**Integration:**
- Automatically adds item to player's InventoryComponent
- Notifies QuestSubsystem via QuestTargetComponent
- Destroys actor or disables based on settings

---

### 5. AWorldItem (Actor)
**Purpose:** Physical representation of item in world

**Components:**
- UStaticMeshComponent: Visual mesh
- USphereComponent: Collision for pickup
- UPickupComponent: Pickup logic
- UQuestTargetComponent: Quest integration

**Setup:**
- Designer places in level
- Assigns ItemData (DataAsset reference)
- Configures pickup settings
- Item automatically integrates with inventory and quests

---

### 6. UInteractableDefinition (DataAsset)
**Purpose:** Defines properties of interactable objects

**Properties:**
- InteractableType: Door, Chest, Lever, Button, etc.
- InteractionPrompt: "Press E to open door"
- InteractionDistance: How close player must be
- InteractionDuration: Instant or hold-to-interact
- bRequiresItem: Does it need an item to interact?
- RequiredItemID: Which item is needed (e.g., key)
- RequiredTags: GameplayTags for requirements
- QuestTags: Tags for quest integration

**Examples:**
- DA_Interactable_LockedDoor
- DA_Interactable_TreasureChest
- DA_Interactable_Lever

---

### 7. UInteractionComponent (Component)
**Purpose:** Handles interaction logic for objects

**Attached to:** AInteractableObject actors

**Properties:**
- InteractableData: Reference to UInteractableDefinition
- bHasBeenInteracted: Track if already used
- InteractionCount: How many times interacted
- QuestTarget: Reference to UQuestTargetComponent

**API:**
- Interact(): Perform interaction
- CanInteract(): Check if can interact
- GetInteractionPrompt(): Get UI prompt text

**Events:**
- OnInteracted: Broadcast when interacted

**Integration:**
- Checks player inventory for required items
- Consumes items if needed
- Notifies QuestSubsystem
- Triggers Blueprint events for custom behavior

---

### 8. AInteractableObject (Actor)
**Purpose:** Base class for doors, chests, levers, etc.

**Components:**
- UStaticMeshComponent: Visual mesh
- UInteractionComponent: Interaction logic
- UQuestTargetComponent: Quest integration

**Interfaces:**
- IQuestInteractable: For quest system integration

**Behavior:**
- Designer configures via InteractableDefinition
- Blueprint events for custom behavior (door opening animation, etc.)
- Automatic quest integration

---

## 🔄 Data Flow Examples

### Example 1: Pickup Item → Inventory → Quest

```
1. Player walks near AWorldItem (Health Potion)
   │
   ▼
2. UPickupComponent detects overlap
   │
   ▼
3. PickupComponent->PickupItem(Player)
   │
   ▼
4. Player->InventoryComponent->AddItem(DA_Item_HealthPotion, 1)
   │
   ├─> Check: HasSpace? ✓
   ├─> Check: Can stack? ✓
   ├─> Add to inventory
   ├─> OnInventoryChanged.Broadcast()
   │   └─> UI updates
   │
   ▼
5. PickupComponent->QuestTarget->NotifyQuestSystem()
   │
   ▼
6. QuestSubsystem checks active quests
   │
   ▼
7. QuestTask_Collect->UpdateProgress(1)
   │
   ▼
8. Quest UI updates: "Collect Potions: 3/5"
```

### Example 2: Use Key → Unlock Door → Quest

```
1. Player approaches locked door
   │
   ▼
2. Player presses "E"
   │
   ▼
3. InteractionComponent->Interact(Player)
   │
   ▼
4. Check: RequiresItem? ✓ (Golden Key)
   │
   ▼
5. Player->InventoryComponent->HasItem(DA_Item_GoldenKey)? ✓
   │
   ▼
6. Door unlocks
   │
   ├─> InventoryComponent->RemoveItem(DA_Item_GoldenKey, 1)
   ├─> OnInventoryChanged.Broadcast()
   ├─> Blueprint: PlayDoorOpenAnimation()
   ├─> PlaySound: UnlockSound
   │
   ▼
7. InteractionComponent->QuestTarget->NotifyQuestSystem()
   │
   ▼
8. QuestTask_Interact->UpdateProgress(1)
   │
   ▼
9. Quest completes: "Unlock the Ancient Door"
```

### Example 3: Use Consumable → Restore Health

```
1. Player opens inventory UI
   │
   ▼
2. Player clicks "Use" on Health Potion
   │
   ▼
3. InventoryComponent->UseItem(DA_Item_HealthPotion)
   │
   ▼
4. Check: ItemData->bIsUsable? ✓
   │
   ▼
5. Apply effects
   │
   ├─> Player->Health += ItemData->HealthRestore
   ├─> PlaySound: DrinkSound
   ├─> PlayEffect: HealingParticles
   │
   ▼
6. Check: ItemData->bConsumeOnUse? ✓
   │
   ▼
7. InventoryComponent->RemoveItem(DA_Item_HealthPotion, 1)
   │
   ▼
8. OnInventoryChanged.Broadcast()
   │
   ▼
9. UI updates inventory display
```

---

## 🎮 Quest System Integration

### GameplayTags for Items

```
Item
├─ Item.Type.Consumable
│  ├─ Item.Type.Consumable.Potion
│  └─ Item.Type.Consumable.Food
├─ Item.Type.Weapon
│  ├─ Item.Type.Weapon.Pistol
│  └─ Item.Type.Weapon.Rifle
├─ Item.Type.Ammo
├─ Item.Type.Key
│  ├─ Item.Type.Key.Golden
│  └─ Item.Type.Key.Silver
└─ Item.Quest
   ├─ Item.Quest.MainStory
   └─ Item.Quest.SideQuest
```

### Quest Task Integration

**UQuestTask_Collect:**
- Tracks items by ItemID or ItemTags
- Checks InventoryComponent for item count
- Updates progress when items are added to inventory

**UQuestTask_Interact:**
- Tracks interactions by QuestTags
- Notified via InteractionComponent
- Can require specific items to complete

**Example Quest:**
```json
{
  "QuestID": "Q_FindTheKey",
  "Objectives": [
    {
      "Description": "Find the Golden Key",
      "Tasks": [
        {
          "Type": "Collect",
          "ItemID": "Item_GoldenKey",
          "TargetCount": 1
        }
      ]
    },
    {
      "Description": "Unlock the Ancient Door",
      "Tasks": [
        {
          "Type": "Interact",
          "QuestTags": ["Interactable.Door.Ancient"],
          "TargetCount": 1
        }
      ]
    }
  ]
}
```

---

## 📁 File Structure

```
Source/cplus/
├─ ItemSystem/
│  ├─ Data/
│  │  ├─ ItemDefinition.h/cpp           (DataAsset for items)
│  │  ├─ InteractableDefinition.h/cpp   (DataAsset for interactables)
│  │  └─ InventoryItem.h                (Struct for item instances)
│  │
│  ├─ Components/
│  │  ├─ InventoryComponent.h/cpp       (Inventory management)
│  │  ├─ PickupComponent.h/cpp          (Pickup logic)
│  │  └─ InteractionComponent.h/cpp     (Interaction logic)
│  │
│  └─ Actors/
│     ├─ WorldItem.h/cpp                (Pickup actor)
│     └─ InteractableObject.h/cpp       (Interactable actor)
│
├─ QuestSystem/
│  └─ (existing quest system files)
│
└─ Variant_Shooter/
   └─ ShooterCharacter.h/cpp            (+ InventoryComponent)
```

---

## 🎨 Designer Workflow

### Creating a New Item

1. **Create ItemDefinition DataAsset:**
   - Right-click in Content Browser
   - Miscellaneous → Data Asset → ItemDefinition
   - Name: DA_Item_HealthPotion

2. **Configure Item Properties:**
   - ItemID: "Item_HealthPotion"
   - ItemType: Consumable
   - ItemName: "Health Potion"
   - ItemDescription: "Restores 50 health"
   - ItemIcon: [select texture]
   - ItemTags: "Item.Type.Consumable.Potion"
   - MaxStackSize: 10
   - bIsUsable: true
   - bConsumeOnUse: true
   - HealthRestore: 50.0
   - WorldMesh: [select mesh]

3. **Place in World:**
   - Drag AWorldItem into level
   - Set PickupComponent->ItemData = DA_Item_HealthPotion
   - Set PickupComponent->ItemQuantity = 1
   - Done! Item is now pickupable and integrates with quests

### Creating a Locked Door

1. **Create InteractableDefinition DataAsset:**
   - Right-click → Data Asset → InteractableDefinition
   - Name: DA_Interactable_LockedDoor

2. **Configure Interactable:**
   - InteractableType: Door
   - InteractionPrompt: "Press E to unlock door"
   - bRequiresItem: true
   - RequiredItemID: "Item_GoldenKey"
   - QuestTags: "Interactable.Door.Ancient"

3. **Place in World:**
   - Drag AInteractableObject into level
   - Set InteractionComponent->InteractableData = DA_Interactable_LockedDoor
   - Set mesh and collision
   - Create Blueprint event for door opening animation
   - Done! Door requires key and integrates with quests

---

## 🔧 Implementation Checklist

### Phase 1: Core Data (Priority 1)
- [ ] UItemDefinition (DataAsset)
- [ ] FInventoryItem (Struct)
- [ ] UInteractableDefinition (DataAsset)

### Phase 2: Components (Priority 1)
- [ ] UInventoryComponent
- [ ] UPickupComponent
- [ ] UInteractionComponent

### Phase 3: Actors (Priority 1)
- [ ] AWorldItem
- [ ] AInteractableObject

### Phase 4: Integration (Priority 2)
- [ ] Add InventoryComponent to AShooterCharacter
- [ ] Update QuestTask_Collect to check inventory
- [ ] Update QuestTask_Interact to work with InteractionComponent
- [ ] Create example items and interactables

### Phase 5: UI (Priority 2)
- [ ] Inventory UI widget
- [ ] Item tooltip widget
- [ ] Interaction prompt widget

### Phase 6: Testing (Priority 3)
- [ ] Test pickup → inventory flow
- [ ] Test item usage
- [ ] Test quest integration
- [ ] Test item requirements for interactions

---

## 📊 Benefits of This Architecture

### Scalability
- ✅ Add new items by creating DataAssets (no code)
- ✅ Add new interactables by creating DataAssets (no code)
- ✅ Components are reusable across all actors

### Flexibility
- ✅ Same actor can be pickup AND interactable
- ✅ Change item behavior by swapping DataAsset
- ✅ Quest integration is automatic via GameplayTags

### Designer-Friendly
- ✅ Create items in Editor without C++
- ✅ Configure behavior visually
- ✅ Instant iteration (no rebuilds)

### Quest Integration
- ✅ Automatic tracking via QuestTargetComponent
- ✅ GameplayTags for flexible matching
- ✅ Event-driven notifications

### Maintainability
- ✅ Data separated from logic
- ✅ Clear component responsibilities
- ✅ Easy to debug and test

---

## 🔗 Integration with Existing Systems

### Quest System
- Items use GameplayTags for quest matching
- InventoryComponent notifies QuestSubsystem
- QuestTask_Collect checks inventory
- QuestTask_Interact works with InteractionComponent

### Shooter Variant
- AShooterCharacter has InventoryComponent
- Weapons can be items in inventory
- Ammo managed through inventory
- Health/armor pickups use item system

### Save System (Future)
- FInventoryItem is serializable
- Save/load inventory state
- Persist item instances with custom data

---

## 📚 References

- [Unreal Engine Data Assets](https://docs.unrealengine.com/5.7/en-US/data-assets-in-unreal-engine/)
- [Component-Based Design](https://docs.unrealengine.com/5.7/en-US/components-in-unreal-engine/)
- [Gameplay Tags](https://docs.unrealengine.com/5.7/en-US/using-gameplay-tags-in-unreal-engine/)
- Quest System Architecture (QUEST_SYSTEM_ARCHITECTURE.md)

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
