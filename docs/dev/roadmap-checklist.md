# Inventory + Weapon + Quest Integration Roadmap (Checklist)

## Scope

**Goal:** Implement a unified item architecture where all items can be looted, rewarded, picked up, bought, sold, and dropped **except Quest Items** which are special.

**Systems involved:**
- InventorySystem (`Source/cplus/InventorySystem`)
- WeaponSystem (`Source/cplus/WeaponSystem`)
- QuestSystem (`Source/cplus/QuestSystem`)

**Key decisions (locked):**
- **Ammo is a normal inventory item** (visible, lootable, tradable).
- **Weapons are both**:
  - A normal inventory item (loot/trade/drop)
  - An equipped runtime actor managed by WeaponSystem
- **Quest items use the same inventory storage** (`UInventoryComponent::Items`) but are special:
  - Not buyable / sellable
  - Not droppable
  - Not destroyable

---

## Global Invariants

- Inventory is the single storage for items: `UInventoryComponent::Items`.
- GameplayTags are used for:
  - UI filtering/subcategories
  - Quest matching
  - Ammo type mapping
- Avoid duplicated event notifications between Inventory/Pickup/Quest.
- Prefer event-driven integration (delegates), avoid polling/tick.

---

## UI Architecture (Do This Early)

### Goals

- Provide a consistent UX for:
  - Inventory browsing (tabs by `EItemType`, search, sort)
  - Item actions (Use, Equip, Drop, Split Stack)
  - Trading (Buy/Sell) for all non-quest items
  - Quest item restrictions (no buy/sell/drop/destroy)
  - Weapon HUD (current weapon + magazine + reserve ammo)
  - Quest HUD (already exists via `UQuestUIManager`)
- Use event-driven updates:
  - Inventory changes drive UI refresh via delegates
  - Weapon changes/ammo changes drive HUD via delegates
  - Quest updates drive quest widgets via `UQuestUIManager`

### UI Managers (C++)

**Existing**
- `UUIManager` (global UI manager)
- `UQuestUIManager` (quest notifications + journal)

**To add**
- `UInventoryUIManager` (inventory screen + inventory notifications)
- `UShopUIManager` (vendor/shop screen)

**Integration point**
- `UUIManager` owns sub-managers:
  - Quest UI (already)
  - Inventory UI (new)
  - Shop UI (new)

### Widgets (Blueprint first, C++ optional)

**Inventory**
- `WBP_InventoryScreen` (full screen inventory)
- `WBP_InventoryGrid` (list/grid container)
- `WBP_InventoryItemSlot` (single item slot)
- `WBP_ItemTooltip` (hover details)
- `WBP_SplitStackDialog` (split stack)
- `WBP_ConfirmDialog` (drop/destroy confirmations)

**Shop/Trade**
- `WBP_ShopScreen` (vendor UI)
- `WBP_ShopItemRow` (buy/sell row)
- `WBP_TradeConfirmDialog` (confirm purchase/sale)

**Weapon HUD**
- `WBP_WeaponHUD` (weapon name + ammo)
- `WBP_AmmoRow` (optional, show multiple reserve ammo types)

### UI Data Flow (Contracts)

**Inventory -> UI**
- Bind to `UInventoryComponent::OnInventoryChanged`
- Bind to `UInventoryComponent::OnInventoryFull`
- Bind to `UInventoryComponent::OnItemUsed`

**Weapon -> UI**
- Bind to `UWeaponComponent::OnWeaponChanged`
- Bind to `UWeaponComponent::OnAmmoChanged`
- Bind to `UWeaponComponent::OnReserveAmmoChanged`

**Quest -> UI**
- Already handled by `UQuestUIManager` binding to `UQuestSubSystem` delegates

### UI Action Rules (Hard Requirements)

- Quest items:
  - must not show Buy/Sell options in shop
  - must not allow Drop/Destroy
- Non-quest items:
  - must allow Drop
  - must allow Buy/Sell
- All item actions must call `UInventoryComponent` APIs (single source of truth)

### UI Verification

- [ ] Inventory screen opens/closes and does not break input mode
- [ ] Inventory updates immediately after pickup/reward/buy/sell/drop
- [ ] Quest items: no Buy/Sell/Drop actions available
- [ ] Weapon HUD updates when weapon changes and when ammo changes

---

## Tag & Type Standard (Do This First)

### ItemType (Primary Category)

Use `EItemType` as the primary category for UI tabs and high-level behavior.
- `Consumable`
- `Weapon`
- `Armor`
- `Ammo`
- `QuestItem`
- `Material` (CraftItems)
- `Misc`

### ItemTags (Secondary Category)

Define a strict tag convention.

**Weapons**
- `Item.Weapon`
- `Item.Weapon.Ranged`
- `Item.Weapon.Ranged.Pistol`
- `Item.Weapon.Ranged.Rifle`
- `Item.Weapon.Melee`

**Ammo**
- `Item.Ammo`
- `Item.Ammo.Pistol`
- `Item.Ammo.Rifle`
- `Item.Ammo.Shotgun`
- `Item.Ammo.Sniper`
- `Item.Ammo.Explosive`
- `Item.Ammo.Energy`
- `Item.Ammo.Special`

**Armor**
- `Item.Armor`
- `Item.Armor.Head`
- `Item.Armor.Chest`
- `Item.Armor.Legs`

**Consumables**
- `Item.Consumable`
- `Item.Consumable.Health`
- `Item.Consumable.Stamina`

**CraftItems**
- `Item.Craft`
- `Item.Craft.Metal`
- `Item.Craft.Herb`

**Quest Items**
- `Item.Quest`
- `Item.Quest.Main`
- `Item.Quest.Side`

**Checklist**
- [x] Confirm `DefaultGameplayTags.ini` contains the required tag tree.
- [ ] Create/adjust at least 1 example ItemDefinition per category with correct `ItemType` + `ItemTags`.

**Definition of Done**
- A designer can create a new item and place it into exactly one `EItemType` and multiple `ItemTags` under the convention above.

---

## Phase 1 — Quest Item Policy (No Trading / No Dropping)

### Goal
Quest items are stored in the same inventory, but cannot be bought/sold/dropped/destroyed.

### Tasks
- [x] In `UItemDefinition`, enforce authoring rules in data:
  - [x] Quest items must have: `ItemType = QuestItem`.
  - [x] Quest items must have: `bIsQuestItem = true`.
  - [x] Quest items must have: `bCanBeDropped=false`, `bCanBeSold=false`, `bCanBeDestroyed=false`.
- [x] In `UInventoryComponent` enforce policy in code:
  - [x] Block `DropItem()` for `bIsQuestItem`.
  - [x] Block any future `SellItem()` for `bIsQuestItem`.
- [ ] UI policy:
  - [ ] Disable Drop/Sell buttons for quest items.

### Files
- `Source/cplus/InventorySystem/ItemDefinition.h`
- `Source/cplus/InventorySystem/InventoryComponent.h/.cpp`
- (UI files TBD)

### Verification
- [ ] Pick up a quest item.
- [ ] Attempt to drop it (should fail).
- [ ] Attempt to sell it in shop UI (when implemented) (should fail).

---

## Phase 2 — Unify Item Acquisition Sources

### Goal
All item acquisition funnels through `UInventoryComponent::AddItem()`.

### Sources to support
- Pickup (world)
- Quest reward
- Shop buy
- Scripted grant

### Tasks
- [x] Ensure all producers call `Inventory->AddItem(ItemData, Quantity)`.
- [x] Create consistent result handling (partial add due to slot/weight caps).

### Verification
- [ ] Pickup adds to inventory.
- [ ] Reward adds to inventory.
- [ ] Shop buy adds to inventory.

---

## Phase 2.5 — World Item System

### Goal
A generic `AWorldItem` actor that can represent any item in the world (dropped, placed, loot).

### Tasks
- [ ] Create `AWorldItem` actor with:
  - [ ] `UItemDefinition*` reference
  - [ ] `int32 Quantity`
  - [ ] `UPickupComponent` for interaction
  - [ ] Mesh from `ItemDefinition->WorldMesh`
  - [ ] Physics enabled for dropped items
- [ ] Update `DropItem()` to spawn `AWorldItem` at owner location
- [ ] Migrate/replace `AWeaponPickup` to use `AWorldItem` with weapon ItemDefinition

### Files
- `Source/cplus/InventorySystem/WorldItem.h/.cpp` (new)
- `Source/cplus/InventorySystem/InventoryComponent.cpp` (update DropItem)
- `Source/cplus/WeaponSystem/Actors/WeaponPickup.h/.cpp` (deprecate or migrate)

### Verification
- [ ] Dropping an item spawns a pickup in the world
- [ ] Picking up the spawned item adds it back to inventory
- [ ] Weapon pickups work through the new system

---

## Phase 2.8 — Shop System

### Goal
Enable buying/selling items from NPC vendors.

### Tasks
- [ ] Create `UShopComponent` for NPC vendors:
  - [ ] `TArray<FShopItem> Inventory` (item + price + stock)
  - [ ] `BuyItem(item, qty)` → deduct gold, call AddItem
  - [ ] `SellItem(item, qty)` → call RemoveItem, add gold
  - [ ] Price modifiers (reputation, skills)
- [ ] Create `FShopItem` struct (ItemDefinition, BasePrice, Stock, bUnlimitedStock)
- [ ] Add gold/currency to player (via InventoryComponent or separate)
- [ ] Quest items blocked from sell (already done in SellItem)

### Files
- `Source/cplus/InventorySystem/ShopComponent.h/.cpp` (new)
- `Source/cplus/InventorySystem/ShopTypes.h` (new, structs)

### Verification
- [ ] Player can buy item from vendor
- [ ] Player can sell non-quest item to vendor
- [ ] Quest items cannot be sold
- [ ] Gold is deducted/added correctly

---

## Phase 3 — Quest Integration (Collect) via Inventory Only

### Goal
Collect quest progress updates regardless of where an item came from, without double-notify.

### Current risk
- `UPickupComponent` currently notifies quest via `UQuestTargetComponent`.
- `UInventoryComponent::AddItem()` also calls `NotifyQuestSystem()`.

### Tasks
- [x] Choose a single official collect-notify path: **Inventory**.
- [x] Disable/remove collect notify in `UPickupComponent`.
- [x] Ensure `UInventoryComponent::NotifyQuestSystem()` emits tags that `QuestTask_Collect` expects.

### Matching contract
- Collect tasks should match by either:
  - ItemID (exact), or
  - ItemTag (category)

### Files
- `Source/cplus/InventorySystem/PickupComponent.h/.cpp`
- `Source/cplus/InventorySystem/InventoryComponent.h/.cpp`
- `Source/cplus/QuestSystem/Tasks/QuestTask_Collect.h/.cpp`
- `Source/cplus/QuestSystem/Runtime/QuestSubSystem.cpp`

### Verification
- [ ] Quest Collect progresses when:
  - [ ] item is picked up
  - [ ] item is granted as quest reward
  - [ ] item is purchased
- [ ] No duplicate increments for a single pickup.

---

## Phase 4 — Ammo as Inventory Item (Bridge WeaponSystem)

### Goal
Ammo is an inventory item; WeaponComponent consumes/reserves ammo via inventory.

### Tasks
- [ ] Create Ammo `UItemDefinition` assets:
  - [ ] `DA_Item_Ammo_Pistol`
  - [ ] `DA_Item_Ammo_Rifle`
  - [ ] `DA_Item_Ammo_Shotgun`
  - [ ] `DA_Item_Ammo_Sniper`
  - [ ] `DA_Item_Ammo_Explosive`
  - [ ] `DA_Item_Ammo_Energy`
  - [ ] `DA_Item_Ammo_Special`
- [x] Define mapping: `EAmmoType -> Ammo ItemID/ItemTag` (via `AmmoTypeToItem` TMap in WeaponComponent).
- [x] Add a bridge layer in WeaponComponent:
  - [x] Read reserve from inventory quantity (`GetReserveAmmo()`)
  - [x] Consume from inventory when reloading (`ConsumeReserveAmmo()`)
  - [x] Add ammo via inventory when picking up ammo (`AddReserveAmmo()`)

### Files
- `Source/cplus/WeaponSystem/Components/WeaponComponent.h/.cpp`
- `Source/cplus/InventorySystem/InventoryComponent.h/.cpp`
- Content: ItemDefinition assets

### Verification
- [ ] Ammo pickup increases inventory ammo count.
- [ ] Reload consumes ammo from inventory.
- [ ] UI shows reserve ammo derived from inventory.

---

## Phase 5 — Weapons as Inventory Items + Equipped State

### Goal
Weapons are tradeable/droppable items, while equipped weapon is managed by WeaponComponent.

### Tasks
- [x] Decide weapon item data binding:
  - [x] `UItemDefinition` gets a reference to `WeaponClass` (minimal)
  - [ ] `UItemDefinition` references a `UWeaponDefinition` (data-driven) - NOT CHOSEN
- [ ] Create weapon ItemDefinitions (non-stack):
  - [ ] `ItemType = Weapon`
  - [ ] `MaxStackSize = 1`
  - [ ] `bCanBeDropped = true`
  - [ ] tags under `Item.Weapon.*`
- [x] Equip pipeline:
  - [x] Inventory selection triggers WeaponComponent equip (`EquipWeaponItem()`)
  - [x] WeaponComponent spawns/activates weapon actor

### Existing overlap
- `AWeaponPickup` currently calls `WeaponComponent->AddWeaponClass()`.

### Migration options
- [ ] Option A: Replace `AWeaponPickup` with `AWorldItem` using weapon ItemDefinition.
- [ ] Option B: Modify `AWeaponPickup` to grant weapon ItemDefinition via inventory, then auto-equip.

### Files
- `Source/cplus/WeaponSystem/Actors/WeaponPickup.h/.cpp`
- `Source/cplus/WeaponSystem/Components/WeaponComponent.h/.cpp`
- `Source/cplus/InventorySystem/ItemDefinition.h` (if adding weapon reference)

### Verification
- [ ] Weapon pickup adds weapon item to inventory.
- [ ] Player can equip weapon from inventory.
- [ ] Player can drop weapon item and it spawns a world pickup.

---

## Phase 5.5 — Weapon Unequip + Full Cycle

### Goal
Complete weapon lifecycle: pickup → inventory → equip → unequip → drop.

### Tasks
- [ ] Add `UnequipWeaponItem()` to InventoryComponent
- [ ] Track equipped weapon in inventory (flag, not removal)
- [ ] Add `EquippedWeaponItem` property to track current weapon item
- [ ] Drop equipped weapon → unequip first, then drop
- [ ] Sync WeaponComponent state with inventory equipped state

### Files
- `Source/cplus/InventorySystem/InventoryComponent.h/.cpp`
- `Source/cplus/WeaponSystem/Components/WeaponComponent.h/.cpp`

### Verification
- [ ] Player can unequip weapon (goes back to "in inventory" state)
- [ ] Dropping equipped weapon unequips it first
- [ ] Equipped weapon is visually indicated in inventory

---

## Phase 6 — Armor & Consumables (Use + Equip)

### Armor
- [ ] Define armor item tags by slot (`Item.Armor.Head`, etc.).
- [ ] Add Equipment component (recommended) or extend WeaponComponent/Inventory UI in a minimal way.

### Consumables
- [x] `UseItem()` should apply effects based on ItemDefinition fields (Health via HealthComponent).
- [x] Ensure `bConsumeOnUse` removes 1 quantity.

### Verification
- [ ] Consuming a potion applies effect and reduces quantity.
- [ ] Equipping armor updates player stats (when stats system is ready).

---

## Phase 6.5 — Equipment Component

### Goal
Dedicated component for armor/equipment slots with stat modifiers.

### Tasks
- [ ] Create `EEquipmentSlot` enum (Head, Chest, Legs, Hands, Feet, Accessory1, Accessory2)
- [ ] Create `UEquipmentComponent`:
  - [ ] `TMap<EEquipmentSlot, UItemDefinition*> EquippedItems`
  - [ ] `EquipItem(slot, item)` / `UnequipItem(slot)`
  - [ ] `GetEquippedItem(slot)`
  - [ ] `OnEquipmentChanged` delegate
- [ ] Add `EquipmentSlot` property to `UItemDefinition` for armor items
- [ ] Stat modifiers from equipped items (Armor, Health bonus, etc.)
- [ ] Integration with InventoryComponent (mark items as equipped)

### Files
- `Source/cplus/InventorySystem/EquipmentComponent.h/.cpp` (new)
- `Source/cplus/InventorySystem/EquipmentTypes.h` (new, enums/structs)
- `Source/cplus/InventorySystem/ItemDefinition.h` (add EquipmentSlot)

### Verification
- [ ] Player can equip armor to correct slot
- [ ] Equipped armor shows stat bonuses
- [ ] Cannot equip wrong item type to slot
- [ ] Equipment persists in save/load

---

## Phase 7 — Crafting (Materials -> Recipes)

### Goal
Craft items (Material) can be consumed to craft other items.

### Tasks
- [x] Create `UCraftingRecipe` DataAsset (inputs: ItemID/Tag + qty; outputs: ItemID + qty).
- [x] Create `UCraftingComponent` using `InventoryComponent` to check/consume materials.

### Verification
- [ ] Craft consumes correct materials.
- [ ] Craft result is added to inventory.

---

## Phase 8 — Save/Load + UI

### Save/Load
- [x] Persist `Items` and quantities (`GetSaveData()`, `LoadFromSaveData()`).
- [x] Persist `CustomData` and `InstanceID` if needed.
- [x] Persist equipped weapon state (`FWeaponSaveData` in SaveGameData.h).

### UI
- [ ] Inventory screen with tabs by `EItemType`.
- [ ] Filter by tags inside tabs.
- [ ] Disable actions for quest items.
- [ ] Shop screen:
  - [ ] Buy flow adds items via `AddItem()`
  - [ ] Sell flow removes items via `RemoveItem()`
  - [ ] Quest items cannot be bought/sold
- [ ] Weapon HUD:
  - [ ] magazine + reserve ammo
  - [ ] updates via WeaponComponent delegates
- [ ] Interaction with input modes:
  - [ ] UIOnly/GameAndUI focus works
  - [ ] inventory does not conflict with quest dialog widgets

---

## Phase 8.5 — Inventory UI Helpers

### Goal
Provide C++ helper functions for UI to query and display inventory.

### Tasks
- [ ] Add to `UInventoryComponent`:
  - [ ] `GetItemsByType(EItemType)` → filtered array
  - [ ] `GetItemsByTag(FGameplayTag)` → filtered array
  - [ ] `SortItems(ESortType)` enum (Name, Type, Rarity, Weight, Value)
  - [ ] `GetTotalWeight()` / `GetWeightCapacity()` / `GetRemainingCapacity()`
  - [ ] `GetItemCount()` → total unique item stacks
  - [ ] `IsEmpty()` / `IsFull()`
- [ ] Add `ESortType` enum
- [ ] Add weight system properties (CurrentWeight, MaxWeight, bUseWeightLimit)

### Files
- `Source/cplus/InventorySystem/InventoryComponent.h/.cpp`
- `Source/cplus/InventorySystem/InventoryTypes.h` (new, ESortType)

### Verification
- [ ] UI can filter items by type
- [ ] UI can filter items by tag
- [ ] UI can sort items
- [ ] Weight display works correctly

---

## Definition of Done (Overall)
- [ ] All item categories exist as ItemDefinitions. (DataAssets need to be created in Editor)
- [x] Ammo is fully inventory-backed. (C++ bridge complete, need DataAssets)
- [x] Weapons are inventory items and equippable. (C++ complete, need DataAssets)
- [x] Quest items cannot be sold/dropped/destroyed.
- [x] Collect quests progress from any item acquisition source.
- [x] No duplicate quest progress events.
- [x] Save/Load preserves inventory and equipped state.
