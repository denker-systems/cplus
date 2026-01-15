---
description: Commit UE 5.7 code with conventional commits
---
# Git Commit Workflow


## 1. Check & Review

// turbo
```powershell
git status --short
git diff --stat
```

**Check:** No sensitive data, no build artifacts, .Build.cs updated

## 2. Stage

```powershell
git add .  # or specific: git add Source/cplus/[path]
```

## 3. Commit

**Format:** `<type>(<scope>): <description>`

**Types:** feat, fix, docs, refactor, chore  
**Scopes:** core, quest, shooter, horror, ai, ui, build

**Examples:**
```bash
feat(quest): Add QuestGiverComponent
fix(ai): NPC navigation failing
chore(build): Add GameplayTags dependency
```

```powershell
git commit -m "feat(quest): Add QuestTrackerComponent"
```

## 4. Update Docs

Add to CHANGELOG.md under `## [Unreleased]` and update session report.

## 5. Verify

```powershell
git log --oneline -5
# Undo: git commit --amend  or  git reset --soft HEAD~1
```

## 6. Push (⚠️ Ask user first!)

```powershell
# Verify build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Show commits
git log --oneline origin/main..HEAD
```

**Rules:** NEVER push/force-push without permission. Always verify build.

---

**v2.1 Ultra-optimized | 2026-01-14**
