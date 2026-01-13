---
trigger: always_on
---
# Workspace Rules for cplus Project

> **Destination:** `.windsurf/rules/workspace-rules.md`

---
trigger: always_on
description: Core workspace rules for Unreal Engine 5.7 C++ development
---

# Workspace Rules

> Essential rules that apply to all development in this workspace

## 🎯 Project Context

### Project Information
- **Project Name:** cplus
- **Engine:** Unreal Engine 5.7
- **Language:** C++
- **Type:** Multi-variant game (Shooter + Horror)
- **Core System:** Quest System with components and subsystem
- **Repository:** https://github.com/denker-systems/cplus
- **Branch Strategy:** Feature branches from main/master

### Technology Stack
```xml
<technology_stack>
- Engine: Unreal Engine 5.7
- Language: C++
- Build System: Unreal Build Tool (UBT)
- Version Control: Git with Git LFS
- IDE: Windsurf Editor
- Documentation: Markdown with Windsurf workflows
</technology_stack>
```

---

## 💬 Communication

### Language Rules
```xml
<language_rules>
- User communication: Swedish
- Code comments: English
- Documentation: English
- Commit messages: English
- Variable names: English
- Function names: English
- All technical writing: English
</language_rules>
```

**Example:**
```cpp
// ✅ CORRECT - English comments
// Calculate quest progress based on completed objectives
void UQuestSubsystem::UpdateProgress(FName QuestID)

// ❌ WRONG - Swedish comments
// Beräkna quest progress baserat på färdiga objectives
void UQuestSubsystem::UpdateProgress(FName QuestID)
```

---

## 🔒 Git Policy - CRITICAL RULES

### Absolute Prohibitions
```xml
<git_critical_rules>
1. NEVER run git commit without explicit user instruction
2. NEVER run git push without explicit user instruction
3. NEVER run git add without explicit user instruction
4. NEVER force push (git push --force)
5. NEVER modify Git history without permission
6. ALWAYS ask before any write operation to Git
</git_critical_rules>
```

### Allowed Git Operations (Read-Only)
```bash
# These are safe and can be run without asking:
git status              # Check repository status
git log                 # View commit history
git diff                # View changes
git branch              # List branches
git remote -v           # View remotes
git show                # Show commit details
```

### Operations Requiring Permission
```bash
# ALWAYS ask user before running these:
git add                 # Stage files
git commit              # Create commit
git push                # Push to remote
git pull                # Pull from remote
git merge               # Merge branches
git checkout            # Switch branches
git stash               # Stash changes
```

---

## 🏗️ Unreal Engine Standards

### Naming Conventions
```xml
<unreal_naming>
- Classes: Prefix + PascalCase
  - AActor: AShooterCharacter, AShooterNPC
  - UObject: UQuestSubsystem, UQuestTrackerComponent
  - FStruct: FQuestObjective, FQuestReward
  - EEnum: EQuestState, EQuestType
  - IInterface: IQuestInteractable, IQuestKillable

- Functions: PascalCase
  - AcceptQuest(), CompleteQuest(), UpdateProgress()

- Variables: PascalCase for UPROPERTY
  - QuestID, CurrentCount, TargetCount

- Booleans: b prefix
  - bIsMandatory, bIsComplete, bCanFail

- Constants: k prefix
  - kMaxQuestCount, kDefaultReward
</unreal_naming>
```

### Memory Management
```xml
<memory_management>
- Use TObjectPtr<> for UPROPERTY UObject pointers
- Use TArray<> instead of std::vector
- Use TMap<> instead of std::map
- Use FString instead of std::string
- Use FName for constant strings
- NEVER use raw new/delete for UObjects
- ALWAYS use UPROPERTY for UObject references
</memory_management>
```

### Code Structure
```xml
<code_structure>
- Use Component pattern for modular functionality
- Use Subsystems for global managers
- Use Interfaces for polymorphic behavior
- Use DataAssets for data-driven design
- Use Delegates for event communication
- Avoid Tick when possible (use events)
- Cache expensive operations
</code_structure>
```

---

## 📝 Documentation Standards

### Code Documentation
```cpp
/**
 * Quest Tracker Component
 * 
 * Tracks player's active quests and broadcasts quest-related events.
 * Integrates with QuestSubsystem for centralized quest management.
 * 
 * @see UQuestSubsystem
 * @see UQuestDefinition
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestTrackerComponent : public UActorComponent
{
    /**
     * Accept a new quest and add to active quests
     * 
     * @param Quest The quest definition to accept
     * @return True if quest was successfully accepted
     */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool AcceptQuest(UQuestDefinition* Quest);
};
```

### Documentation Sync
```xml
<documentation_sync>
After every significant change:
- Update docs/CHANGELOG.md under [Unreleased]
- Update docs/dev/sessions/YYYY-MM-DD.md
- Update docs/dev/DEVLOG_YYYY-MM.md weekly
- Update relevant API documentation
- Keep ROADMAP.md current
</documentation_sync>
```

---

## 🔨 Build System

### Build Commands
```powershell
# Development build (most common)
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Clean build
Remove-Item -Recurse -Force "Intermediate","Binaries" -ErrorAction SilentlyContinue
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

### Build Verification
```xml
<build_rules>
- ALWAYS verify build works before suggesting commit
- Run Development build configuration
- Check for compiler errors and warnings
- Verify no UHT (Unreal Header Tool) errors
- Test that editor starts without crash
</build_rules>
```

---

## 🎮 Quest System Architecture

### Core Components
```xml
<quest_system>
Core Classes:
- UQuestSubsystem: Global quest manager (UGameInstanceSubsystem)
- UQuestDefinition: Quest data (UPrimaryDataAsset)
- UQuestTask: Base class for quest objectives

Components:
- UQuestTrackerComponent: Player quest tracking
- UQuestGiverComponent: NPC quest offering
- UQuestTargetComponent: Quest objective targets

Interfaces:
- IQuestInteractable: Objects that can be interacted with
- IQuestKillable: Objects that can be killed for quests

Task Types:
- UQuestTask_Kill: Kill X enemies
- UQuestTask_Collect: Collect X items
- UQuestTask_Interact: Interact with X objects
- UQuestTask_Location: Reach location
</quest_system>
```

---

## 🚫 Forbidden Practices

### Code Prohibitions
```xml
<forbidden_code>
- std::string, std::vector, std::map (use Unreal types)
- new/delete for UObjects (use NewObject, SpawnActor)
- Raw pointers to UObjects without UPROPERTY
- Static UObject pointers
- Multiple inheritance (except interfaces)
- dynamic_cast (use Cast<>)
- Exceptions (Unreal doesn't use them)
- Unnecessary Tick functions
- Hardcoded data (use DataAssets)
</forbidden_code>
```

### Git Prohibitions
```xml
<forbidden_git>
- Committing Binaries/ directory
- Committing Intermediate/ directory
- Committing Saved/ directory
- Committing .vs/ or IDE files
- Committing without testing build
- Committing sensitive data (API keys, passwords)
- Force pushing to main/master
- Pushing without user permission
</forbidden_git>
```

---

## ✅ Best Practices

### Development Workflow
```xml
<workflow>
1. Create feature branch: feature/[name]
2. Make changes with proper naming
3. Test build (Development configuration)
4. Update documentation
5. Ask user if ready to commit
6. User commits via GitHub Desktop
7. User pushes when ready
</workflow>
```

### Code Quality
```xml
<quality_standards>
- Write self-documenting code
- Add comments for complex logic
- Use descriptive variable names
- Keep functions focused (Single Responsibility)
- Prefer composition over inheritance
- Use const correctness
- Handle edge cases
- Add error checking with check/ensure
</quality_standards>
```

---

## 🔧 AI Assistant Behavior

### What AI Can Do
```xml
<ai_capabilities>
✅ Read and analyze code
✅ Suggest improvements
✅ Write new code
✅ Explain Unreal Engine concepts
✅ Debug issues
✅ Show git status and diffs
✅ Suggest commit messages
✅ Create documentation
✅ Answer questions in Swedish
</ai_capabilities>
```

### What AI Cannot Do Without Permission
```xml
<ai_restrictions>
❌ Run git add
❌ Run git commit
❌ Run git push
❌ Delete files
❌ Modify .gitignore without asking
❌ Change project settings without asking
❌ Install dependencies without asking
❌ Modify build configuration without asking
</ai_restrictions>
```

### AI Must Always
```xml
<ai_requirements>
- Communicate with user in Swedish
- Write all code and documentation in English
- Follow Unreal Engine coding standards
- Ask before any destructive operation
- Verify build works before suggesting commit
- Update documentation with code changes
- Use conventional commit format
- Respect Git policy rules
</ai_requirements>
```

---

## 📊 Project Structure

### Directory Layout
```
cplus/
├── Source/cplus/           # C++ source code
│   ├── QuestSystem/        # Quest system implementation
│   ├── Variant_Shooter/    # Shooter game variant
│   └── Variant_Horror/     # Horror game variant
├── Content/                # Unreal assets (.uasset, .umap)
├── Config/                 # Configuration files
├── docs/                   # Documentation
│   ├── dev/sessions/       # Daily session reports
│   ├── CHANGELOG.md        # Change log
│   └── ROADMAP.md          # Project roadmap
├── .windsurf/              # Windsurf configuration
│   ├── rules/              # Workspace rules
│   └── workflows/          # Development workflows
└── .git/                   # Git repository
```

---

## 🎯 Scopes for Commits

### Conventional Commit Scopes
```xml
<commit_scopes>
- core: Core game systems
- quest: Quest system
- shooter: Shooter variant
- horror: Horror variant
- ai: AI system
- ui: User interface
- build: Build system
- docs: Documentation
- chore: Maintenance tasks
</commit_scopes>
```

**Example Commits:**
```bash
feat(quest): Add quest reward system
fix(ai): NPC navigation stuck in walls
docs(readme): Update installation instructions
chore(build): Update GameplayTags dependency
```

---

## 📚 Quick Reference

### Common Paths
```bash
# Unreal Engine
C:\ue5.7\UE_5.7\Engine

# Project Root
C:\Users\Calle\Documents\Unreal Projects\cplus

# Source Code
C:\Users\Calle\Documents\Unreal Projects\cplus\Source\cplus

# Documentation
C:\Users\Calle\Documents\Unreal Projects\cplus\docs
```

### Common Commands
```powershell
# Build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Git Status
git status --short

# Start Editor
Start-Process "C:\ue5.7\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)  
**Repository:** https://github.com/denker-systems/cplus
