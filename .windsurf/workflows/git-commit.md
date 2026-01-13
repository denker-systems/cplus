# Unreal Engine Git Commit Workflow

> **Destination:** `.windsurf/workflows/git-commit.md`

---
description: Commit Unreal Engine C++ code with conventional commits
auto_execution_mode: 1
---

# Git Commit Workflow

> Commit Unreal Engine code with correct format and documentation

## 1. Check Status - What Has Changed?

// turbo
```powershell
Write-Host "=== GIT STATUS ===" -ForegroundColor Yellow
git status --short
```

### Analyze Changes:
- **M** - Modified (changed files)
- **A** - Added (new files)
- **D** - Deleted (removed files)
- **??** - Untracked (new files not in git)

---

## 2. Review Changes - Check Code

// turbo
```powershell
Write-Host "=== REVIEW CHANGES ===" -ForegroundColor Yellow
git diff --name-only
git diff --stat
```

### Check:
- [ ] No sensitive data (API keys, passwords)
- [ ] No temporary files (Intermediate, Binaries)
- [ ] C++ code follows Unreal standards
- [ ] .Build.cs updated if new dependencies
- [ ] No debug prints or temp code

---

## 3. Stage Changes - Add to Git

### Simple Changes
```powershell
# Add all changes
git add .

# Or specific files
git add Source/cplus/QuestSystem/Core/QuestSubsystem.h
git add Source/cplus/QuestSystem/Core/QuestSubsystem.cpp
```

### Untracked Files
```powershell
# Add new files
git add Source/cplus/QuestSystem/Tasks/QuestTask_Kill.h
git add Source/cplus/QuestSystem/Tasks/QuestTask_Kill.cpp
```

---

## 4. Commit Message Format

### Conventional Commits for Unreal

```
<type>(<scope>): <description>

[optional body]

[optional footer]
```

### Types

| Type | Usage | Example |
|------|-------|---------|
| `feat` | New feature | `feat(quest): Add QuestTrackerComponent` |
| `fix` | Bug fix | `fix(ai): NPC not spawning correctly` |
| `docs` | Documentation | `docs(readme): Update build instructions` |
| `style` | Code style (no logic) | `style(shooter): Fix indentation` |
| `refactor` | Refactoring | `refactor(quest): Simplify quest logic` |
| `test` | Tests | `test(horror): Add unit tests` |
| `chore` | Maintenance | `chore(build): Update .Build.cs` |
| `perf` | Performance | `perf(ai): Optimize pathfinding` |

### Scopes for Unreal Engine

| Scope | Area | Example |
|-------|-------|---------|
| `core` | Core system | `feat(core): Add base character class` |
| `quest` | Quest system | `feat(quest): Implement quest rewards` |
| `shooter` | Shooter variant | `fix(shooter): Weapon reload issue` |
| `horror` | Horror variant | `feat(horror): Add scare system` |
| `ai` | AI system | `fix(ai): NPC stuck in wall` |
| `ui` | User interface | `feat(ui): Add quest journal widget` |
| `build` | Build system | `chore(build): Add GameplayTags dependency` |
| `assets` | Content assets | `feat(assets): Import new weapon mesh` |

---

## 5. Commit Examples

### Feature - New Quest Component
```bash
feat(quest): Add QuestGiverComponent for NPCs

- Implement UQuestGiverComponent with available quests list
- Add OfferQuest() method for player interaction
- Include BlueprintCallable functions for quest management
- Add delegate for quest offer events

Closes #123
```

### Fix - AI Bug
```bash
fix(ai): NPC navigation failing after spawn

- Fix uninitialized NavSystem reference
- Add proper null check before pathfinding
- Set default movement speed in constructor

Fixes crash when AI spawns without navigation mesh
```

### Refactor - Quest System
```bash
refactor(quest): Simplify quest state management

- Remove redundant state tracking in QuestTracker
- Consolidate quest progress in QuestSubsystem
- Update all quest components to use centralized state

No functional changes - code cleanup only
```

### Build - Dependency Update
```bash
chore(build): Add GameplayTags module dependency

- Add "GameplayTags" to cplus.Build.cs
- Required for quest categorization system
```

---

## 6. Commit Command

### Create Commit
```powershell
git commit -m "feat(quest): Add QuestTrackerComponent"
```

### Commit with Body
```powershell
git commit -m "feat(quest): Add QuestTrackerComponent

- Track player's active quests
- Broadcast quest events
- Integrate with QuestSubsystem"
```

### Commit with Footer
```powershell
git commit -m "fix(ai): NPC not spawning correctly

Fix uninitialized spawn timer

Fixes #456"
```

---

## 7. Documentation Sync

### Update Documentation After Commit

#### CHANGELOG.md
```markdown
## [Unreleased]

### Added
- QuestTrackerComponent for tracking player quests
- QuestGiverComponent for NPC quest interactions
- QuestTargetComponent for quest objectives

### Fixed
- AI navigation failing after spawn
- Weapon reload animation timing

### Changed
- Refactored quest state management
```

#### Session Report
```markdown
## Commits This Session

| Time | Hash | Type | Scope | Description |
|-----|------|------|-------|-------------|
| 14:30 | `abc123` | feat | quest | Add QuestTrackerComponent |
| 15:45 | `def456` | fix | ai | NPC navigation fix |
| 16:20 | `ghi789` | chore | build | Add GameplayTags dep |
```

---

## 8. Verify Commit

### Check Commit
```powershell
# Show latest commit
git show --stat HEAD

# Show commit with diff
git show HEAD

# Check log
git log --oneline -5
```

### Undo Commit
```powershell
# If commit is wrong - amend it
git commit --amend -m "feat(quest): Add QuestTrackerComponent (corrected)"

# If commit should be removed completely
git reset --soft HEAD~1
# (changes remain staged)
```

---

## 9. Push Rules

### ⚠️ CRITICAL RULES

1. **NEVER git push without explicit user instruction**
2. **NEVER force push to main/develop**
3. **ALWAYS check build works before push**
4. **ALWAYS verify no sensitive data committed**

### Push Preparation
```powershell
# Check build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -WaitMutex

# Check status
git status

# Show commits to push
git log --oneline origin/main..HEAD
```

---

## 10. File Handling for Unreal Engine

### ✅ DO Commit These Files
- `Source/**/*.h` - C++ headers
- `Source/**/*.cpp` - C++ implementation
- `*.Build.cs` - Build scripts
- `*.uproject` - Project file
- `Config/**/*.ini` - Configuration
- `Content/**/*.uasset` - Blueprint assets
- `Content/**/*.umap` - Level files
- `docs/**/*` - Documentation

### ❌ DO NOT Commit
- `Binaries/**/*` - Compiled DLLs/EXEs
- `Intermediate/**/*` - Build artifacts
- `Saved/**/*` - Saved data, logs
- `.vs/**/*` - VS settings
- `DerivedDataCache/**/*` - Editor cache
- `*.tmp` - Temporary files

### .gitignore for Unreal
```gitignore
# Unreal Engine
Binaries/
Intermediate/
DerivedDataCache/
Saved/
.vs/
*.VC.db
*.VC.opendb
*.sln
*.suo
*.user
*.userosscache
*.sln.docstates

# IDE
.vscode/
.idea/

# OS
.DS_Store
Thumbs.db
```

---

## 11. Branch Strategy

### Branches
- `main` - Production ready
- `develop` - Integration branch
- `feature/quest-system` - Features
- `fix/ai-navigation` - Bug fixes
- `hotfix/critical-bug` - Hotfixes

### Merge Strategy
- **feature branches** → Squash merge to develop
- **develop** → Merge commit to main
- **hotfixes** → Merge to both develop and main

---

## 12. Best Practices

### DO ✅
- Write descriptive commit messages
- Use conventional commits format
- Commit related changes together
- Update documentation
- Check build before commit
- Use proper scope for Unreal systems

### DON'T ❌
- Commit build artifacts (Binaries, Intermediate)
- Commit with "fix stuff" or "wip"
- Commit large changes in one commit
- Push without verifying
- Commit sensitive data
- Forget to update .Build.cs

---

## 13. Quick Reference

### Common Commit Templates

```bash
# New C++ class
feat(scope): Add ClassName component/class

# Bug fix
fix(scope): Fix specific issue description

# Refactor
refactor(scope): Improve/Refactor specific system

# Build dependency
chore(build): Add ModuleName dependency

# Documentation
docs(readme): Update build/install instructions
```

### Git Commands
```powershell
# Status
git status --short

# Stage all changes
git add .

# Commit
git commit -m "feat(quest): Add new quest type"

# Show recent commits
git log --oneline -5

# Undo latest commit
git reset --soft HEAD~1

# Change latest commit message
git commit --amend
```

---

## Commit Checklist

### Before Commit
- [ ] Code compiles (Development build)
- [ ] No build artifacts in changes
- [ ] Commit message follows conventional format
- [ ] Scope is correct for Unreal system
- [ ] Related documentation updated

### After Commit
- [ ] Verify git log looks correct
- [ ] Build still works
- [ ] No sensitive data committed
- [ ] CHANGELOG updated if major change

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
