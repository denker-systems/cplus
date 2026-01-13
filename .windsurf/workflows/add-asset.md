# Unreal Engine Add Asset Workflow

> **Destination:** `.windsurf/workflows/add-asset.md`

---
description: Import and manage Unreal Engine assets
auto_execution_mode: 1
---

# Add Asset Workflow

> Import and manage assets in Unreal Engine 5.7

## 1. Asset Types

### Common Asset Types
| Type | Extension | Description | Example |
|-----|-----------|-------------|---------|
| **Static Mesh** | `.fbx`, `.obj` | 3D models | Weapons, characters |
| **Skeletal Mesh** | `.fbx`, `.obj` | Animated mesh | NPCs, player |
| **Texture** | `.png`, `.jpg`, `.tga` | Textures | Material maps |
| **Material** | `.uasset` | Shader material | PBR materials |
| **Animation** | `.fbx` | Animation clips | Walk, attack |
| **Sound** | `.wav`, `.ogg` | Sound effects | Gunshots, footsteps |
| **Blueprint** | `.uasset` | Visual script | Quest giver |
| **DataAsset** | `.uasset` | Data container | Quest definitions |

---

## 2. Content Browser Organization

### Folder Structure
```
Content/
├── Characters/
│   ├── Player/
│   │   ├── Meshes/
│   │   ├── Materials/
│   │   ├── Animations/
│   │   └── Blueprints/
│   └── NPCs/
│       ├── Shooter/
│       └── Horror/
├── Weapons/
│   ├── Meshes/
│   ├── Materials/
│   ├── Sounds/
│   └── Blueprints/
├── Environment/
│   ├── Props/
│   ├── Architecture/
│   └── Materials/
├── Audio/
│   ├── SFX/
│   ├── Music/
│   └── Voices/
├── UI/
│   ├── Widgets/
│   ├── Icons/
│   └── Materials/
├── Materials/
│   ├── Characters/
│   ├── Weapons/
│   └── Environment/
├── Blueprints/
│   ├── Characters/
│   ├── Game/
│   ├── Quests/
│   └── UI/
└── Data/
    ├── Quests/
    ├── Weapons/
    └── Characters/
```

---

## 3. Import Process

### Import Static Mesh
```powershell
# 1. Open Unreal Editor
# 2. Content Browser → Import
# 3. Select .fbx/.obj file
# 4. Import Settings:
#    - Mesh → Import Mesh: ✓
#    - Mesh → Import Materials: ✓ (if new materials)
#    - Mesh → Import Textures: ✓
#    - Transform → Scale: 1.0 (or match Unreal)
#    - Mesh → Combine Meshes: ✓ (for static props)
# 5. Save in correct folder
```

### Import Skeletal Mesh
```powershell
# Import Settings for skeletal mesh:
# - Skeleton: Select existing skeleton or create new
# - Import Mesh: ✓
# - Import Animations: ✓ (if animations in same file)
# - Mesh → Vertex Color Import Option: Replace
# - Material → Import Materials: ✓
# - Animation → Import Animations: ✓
```

### Import Textures
```powershell
# Texture Import Settings:
# - Compression Settings:
#   • Normal maps: TC_Normalmap
#   • Roughness: TC_Masks
#   • Base Color: TC_Default
#   • Metallic: TC_Masks
#   • AmbientOcclusion: TC_Masks
# - Mip Gen Settings: FromTextureGroup
# - Texture Group: 
#   • Characters: Characters
#   • Weapons: Weapons
#   • World: World
```

---

## 4. Material Creation

### PBR Material Setup
```powershell
# 1. Create new Material
# 2. Add Texture Sample nodes:
#    - BaseColor (sRGB: ✓)
#    - Normal (sRGB: ✗)
#    - Roughness (sRGB: ✗)
#    - Metallic (sRGB: ✗)
#    - AmbientOcclusion (sRGB: ✗)
# 3. Connect to PBR Material pins
# 4. Set Material Domain: Surface
# 5. Blend Mode: Opaque (or Masked/Translucent)
```

### Material Instance
```powershell
# Create Material Instance:
# 1. Right click on Material → Create Material Instance
# 2. Name with "MI_" prefix
# 3. Enable parameter overrides:
#    - Base Color
#    - Roughness
#    - Metallic
#    - Normal intensity
# 4. Save in same folder as parent material
```

---

## 5. Blueprint Assets

### Create Actor Blueprint
```powershell
# 1. Content Browser → Blueprint Class
# 2. Choose Actor (or other base class)
# 3. Name with "BP_" prefix
# 4. Add Components:
#    - Static Mesh/Skeletal Mesh
#    - Collision
#    - Audio components
#    - Custom components (e.g., QuestGiver)
# 5. Configure:
#    - Mesh, material, collision
#    - Event Graph for logic
# 6. Compile and Save
```

### Create DataAsset
```powershell
# 1. Create Blueprint class based on UPrimaryDataAsset
# 2. Name with "DA_" prefix
# 3. Add properties:
#    - Name (FName)
#    - Description (FText)
#    - Arrays of data
#    - GameplayTags
# 4. Fill with data
# 5. Save in Content/Data/[System]/
```

---

## 6. Quest System Assets

### Create Quest Definition
```powershell
# 1. Content/Data/Quests → Right Click → Miscellaneous → Data Asset
# 2. Select QuestDefinition class
# 3. Name: "DA_Quest_[Name]"
# 4. Fill in data:
#    - Quest ID (e.g., "KillBandits")
#    - Title/Description
#    - Objectives (add tasks)
#    - Rewards
#    - Prerequisites
# 5. Save
```

### Create Quest Giver
```powershell
# 1. Create Actor Blueprint: "BP_QuestGiver_[Name]"
# 2. Add:
#    - Skeletal/Static Mesh
#    - QuestGiverComponent
#    - QuestTargetComponent (if can be killed)
# 3. In Event Graph:
#    - OnInteract → OfferQuest
#    - OnKilled → NotifyQuestSystem
# 4. Assign AvailableQuests (DA_Quest_*)
# 5. Place in level
```

---

## 7. Asset Naming Conventions

### Prefixes
| Prefix | Type | Example |
|--------|-----|---------|
| `BP_` | Blueprint | `BP_ShooterNPC` |
| `DA_` | DataAsset | `DA_Quest_KillBandits` |
| `M_` | Material | `M_Character_Body` |
| `MI_` | Material Instance | `MI_Weapon_Pistol` |
| `T_` | Texture | `T_Character_Body_D` |
| `SM_` | Static Mesh | `SM_Weapon_Pistol` |
| `SK_` | Skeletal Mesh | `SK_Character_Player` |
| `A_` | Animation | `A_Character_Walk` |
| `S_` | Sound | `S_Weapon_Fire` |
| `W_` | Widget | `W_QuestJournal` |

### Suffixes
| Suffix | Purpose | Example |
|--------|--------|---------|
| `_D` | Diffuse/Albedo | `T_Body_D` |
| `_N` | Normal | `T_Body_N` |
| `_R` | Roughness | `T_Body_R` |
| `_M` | Metallic | `T_Body_M` |
| `_AO` | Ambient Occlusion | `T_Body_AO` |
| `_E` | Emissive | `T_Sword_E` |
| `_ID` | Icon | `T_Quest_ID` |

---

## 8. Asset Validation

### Import Checklist
```powershell
Write-Host "=== ASSET VALIDATION ===" -ForegroundColor Yellow
Write-Host "□ Naming convention correct"
Write-Host "□ In correct folder structure"
Write-Host "□ Correct import settings"
Write-Host "□ Material assignments correct"
Write-Host "□ Collision setup correct"
Write-Host "□ LODs generated (if needed)"
Write-Host "□ Size/scale correct"
Write-Host "□ No texture stretching"
Write-Host "□ Performance OK (draw calls)"
```

### Performance Checks
```powershell
# In Editor:
stat unit          # Check frame time
stat scenerendering # Draw calls
stat memory        # Memory usage

# Mesh validation:
# - Triangle count reasonable?
# - Material count minimal?
# - LODs setup for complex meshes?
# - Collision optimized?
```

---

## 9. Asset References

### Soft vs Hard References
```cpp
// GOOD - Soft reference (load on demand)
UPROPERTY(EditDefaultsOnly, Category = "Quest")
TSoftObjectPtr<UQuestDefinition> QuestData;

// Load when needed
if (QuestData.IsPending())
{
    QuestData.LoadSynchronous();
}

// BAD - Hard reference (always loaded)
UPROPERTY(EditDefaultsOnly, Category = "Quest")
UQuestDefinition* QuestData;  // Loads immediately
```

### Asset Registry
```cpp
// Find all assets of certain type
FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
TArray<FAssetData> QuestAssets;
AssetRegistry.Get().GetAssetsByPath(TEXT("/Game/Data/Quests"), QuestAssets);
```

---

## 10. Asset Pipeline Automation

### Import Script (Python)
```python
# Import multiple FBX files
import unreal

# Import settings
import_task = unreal.AssetImportTask()
import_task.filename = "C:/path/to/model.fbx"
import_task.destination_path = "/Game/Characters/Player"
import_task.automated = True

# Execute import
unreal.AssetToolsHelpers.get_asset_tools().import_assets([import_task])
```

### Batch Rename
```powershell
# PowerShell script to rename assets
Get-ChildItem "Content\Characters\*.uasset" | ForEach-Object {
    $newName = $_.Name -replace "OldPrefix", "BP_"
    Rename-Item $_.FullName $newName
}
```

---

## 11. Common Issues

### Import Problems
| Problem | Cause | Solution |
|---------|-------|---------|
| Textures blurry | Wrong compression | Change TC_Settings |
| Mesh wrong scale | FBX scale | Change import scale |
| Materials black | Wrong sRGB setting | Normal/Metallic = sRGB off |
| Animation not playing | Wrong retarget | Check skeleton |
| Crash on load | Corrupted asset | Delete and reimport |

### Performance Issues
- **Too many draw calls** - Merge materials
- **High poly count** - Add LODs
- **Large textures** - Reduce resolution
- **No streaming** - Enable texture streaming

---

## 12. Asset Backup

### Source Files
```powershell
# Save source files separately
SourceAssets/
├── Characters/
│   ├── Source/
│   │   ├── .psd (Photoshop)
│   │   ├── .blend/.max/.maya
│   │   └── .fbx (original)
│   └── Textures/
│       └── Source/
│           ├── .psd
│           └── .exr
```

### Version Control
```gitignore
# Ignore temporary files
*.tmp
*.bak
Content/__ExternalActors__/*
Content/__ExternalObjects__/*
Content/__ExternalData__/*
```

---

## Quick Reference

### Import Commands
```powershell
# Import FBX via script
unreal.AssetToolsHelpers.get_asset_tools().import_assets([import_task])

# Reimport asset
unreal.AssetToolsHelpers.get_asset_tools().reimport_assets([asset_data])

# Find assets
unreal.EditorAssetLibrary.list_assets("/Game/Data/Quests")
```

### Common Settings
```powershell
# Static Mesh Import
- Scale: 1.0
- Combine Meshes: ✓
- Generate Lightmap UVs: ✓
- Auto Generate Collision: ✓

# Texture Import
- Compression: TC_Default/Mask/Normalmap
- Mip Gen: FromTextureGroup
- sRGB: ✓ for color, ✗ for data
```

---

## Asset Management Checklist

### Pre-Import
- [ ] Source files ready
- [ ] Naming convention decided
- [ ] Destination folder created
- [ ] Import settings planned

### Post-Import
- [ ] Asset in correct location
- [ ] Name follows convention
- [ ] Materials assigned
- [ ] Collision setup
- [ ] Performance verified
- [ ] Referenced in Blueprints

### Final
- [ ] Save all assets
- [ ] Fix redirectors
- [ ] Update documentation
- [ ] Commit to version control

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
