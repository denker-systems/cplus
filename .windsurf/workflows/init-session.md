# Unreal Engine Init Session Workflow

> **Destination:** `.windsurf/workflows/init-session.md`

---
description: Start a development session with Unreal Engine context loading
auto_execution_mode: 1
---

# Init Session Workflow

> Complete session initialization for Unreal Engine 5.7 C++ project

## 1. Session Info and Timestamp

// turbo
```powershell
$today = Get-Date -Format 'yyyy-MM-dd HH:mm'
$dev = $env:USERNAME
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "=== UNREAL ENGINE SESSION START: $today ===" -ForegroundColor Cyan
Write-Host "=== Developer: $dev ===" -ForegroundColor Cyan
Write-Host "=== Project: cplus (UE 5.7) ===" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
```

---

## 2. Git Status - Check Working Directory

// turbo
```powershell
Write-Host "`n=== GIT STATUS ===" -ForegroundColor Yellow
git status --short
Write-Host "`nCurrent Branch: $(git branch --show-current)" -ForegroundColor Green
```

### Analyze Status
- **Uncommitted changes?** - Commit or stash before new work
- **Untracked files?** - Add to .gitignore or stage
- **Merge conflicts?** - Resolve before continuing
- **Binaries/Intermediate?** - Should be in .gitignore

---

## 3. Recent Commits - Understand Context

// turbo
```powershell
Write-Host "`n=== LAST 10 COMMITS ===" -ForegroundColor Yellow
git log --oneline -10 --format="%h %an: %s (%ar)"
```

### Analyze
- Who worked last?
- Which Unreal systems were affected? (quest, ai, shooter, horror)
- Any ongoing features?
- Any build issues?

---

## 4. Unreal Engine Status

// turbo
```powershell
Write-Host "`n=== UNREAL ENGINE STATUS ===" -ForegroundColor Yellow
$uePath = "C:\ue5.7\UE_5.7"
if (Test-Path $uePath) {
    Write-Host "✅ Unreal Engine 5.7 found at: $uePath" -ForegroundColor Green
} else {
    Write-Host "❌ Unreal Engine 5.7 not found at: $uePath" -ForegroundColor Red
}

# Check if project file exists
$uproject = "cplus.uproject"
if (Test-Path $uproject) {
    Write-Host "✅ Project file found: $uproject" -ForegroundColor Green
} else {
    Write-Host "❌ Project file not found: $uproject" -ForegroundColor Red
}
```

---

## 5. Build Status - Verify Everything Works

// turbo
```powershell
Write-Host "`n=== BUILD CHECK ===" -ForegroundColor Yellow
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -WaitMutex 2>&1 | Select-Object -Last 10
if ($LASTEXITCODE -eq 0) {
    Write-Host "`n✅ BUILD SUCCESSFUL" -ForegroundColor Green
} else {
    Write-Host "`n❌ BUILD FAILED - Fix before continuing!" -ForegroundColor Red
}
```

### If Build Fails
1. Run `git pull` to get latest changes
2. Run clean build if necessary
3. Check .Build.cs dependencies
4. Contact last developer if issue persists

---

## 6. Quest System Status

// turbo
```powershell
Write-Host "`n=== QUEST SYSTEM STATUS ===" -ForegroundColor Yellow
$questFiles = @(
    "Source\cplus\QuestSystem\Core\QuestDefinition.h",
    "Source\cplus\QuestSystem\Core\QuestTask.h",
    "Source\cplus\QuestSystem\Core\QuestSubsystem.h",
    "Source\cplus\QuestSystem\Components\QuestTrackerComponent.h",
    "Source\cplus\QuestSystem\Components\QuestGiverComponent.h",
    "Source\cplus\QuestSystem\Components\QuestTargetComponent.h"
)

foreach ($file in $questFiles) {
    if (Test-Path $file) {
        Write-Host "✅ $file" -ForegroundColor Green
    } else {
        Write-Host "❌ $file" -ForegroundColor Red
    }
}
```

---

## 7. Load Previous Session Report

// turbo
```powershell
Write-Host "`n=== PREVIOUS SESSION ===" -ForegroundColor Yellow
$latest = Get-ChildItem docs/dev/sessions/*.md -ErrorAction SilentlyContinue | Sort-Object Name -Descending | Select-Object -First 1
if ($latest) { 
    Write-Host "File: $($latest.Name)" -ForegroundColor Cyan
    Write-Host "----------------------------------------"
    Get-Content $latest.FullName | Select-Object -First 50
} else {
    Write-Host "No session report found" -ForegroundColor Gray
}
```

### Review
- What was completed last session?
- Any handoff notes?
- Blockers to be aware of?
- Quest system progress?

---

## 8. ROADMAP Progress - Current Phase

// turbo
```powershell
Write-Host "`n=== ROADMAP ===" -ForegroundColor Yellow
Get-Content docs/ROADMAP.md -ErrorAction SilentlyContinue | Select-Object -First 60
```

### Identify
- Which phase is the project in?
- Which tasks are ongoing?
- What's next priority?
- Quest system status?

---

## 9. CHANGELOG - Recent Changes

// turbo
```powershell
Write-Host "`n=== CHANGELOG (Unreleased) ===" -ForegroundColor Yellow
Get-Content docs/CHANGELOG.md -ErrorAction SilentlyContinue | Select-Object -First 40
```

---

## 10. Start Unreal Editor - Smoke Test

```powershell
# Start Unreal Editor
Start-Process -FilePath "C:\ue5.7\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

### Verify
- [ ] Editor starts without crash
- [ ] Project loads correctly
- [ ] C++ classes visible in Content Browser
- [ ] No errors in Output Log
- [ ] Quest system classes available

---

## 11. Create Today's Session Report

Create `docs/dev/sessions/YYYY-MM-DD.md`:

```markdown
# Session YYYY-MM-DD

**Developer:** [Name]
**Branch:** [branch-name]
**Start:** HH:MM
**Focus:** [Today's main task]
**Engine:** Unreal Engine 5.7

---

## Session Goals

### Primary Goals
- [ ] Goal 1 - [Description]
- [ ] Goal 2 - [Description]

### Secondary Goals
- [ ] Goal 3 - [Description]

---

## Progress Log

### HH:MM - Session Start
- Git status: [number of uncommitted files]
- Branch: [branch-name]
- Build status: [OK/FAIL]
- Unreal Engine: 5.7
- Last commit: [hash] [description]

### HH:MM - [Activity]
- [What was done]
- [Result]

---

## Commits This Session

| Time | Hash | Type | Scope | Description |
|-----|------|------|-------|-------------|
| HH:MM | `abc123` | feat | quest | Add QuestTrackerComponent |

---

## Technical Decisions

### Decision 1: [Title]
- **Context:** Why was decision needed?
- **Decision:** What was decided?
- **Consequences:** Impact on system

---

## Unreal Engine Specific

### Quest System
- [ ] Status: [Working/Broken/In Progress]
- [ ] Last change: [What]
- [ ] Next step: [What]

### Build System
- [ ] Build status: [OK/FAIL]
- [ ] Last build: [Time]
- [ ] Known issues: [Description]

---

## Blockers and Problems

| Problem | Status | Solution |
|---------|--------|---------|
| [Problem] | [Resolved/Ongoing/Blocked] | [How it was resolved] |

---

## Code Changes

### New Files
- `Source/cplus/QuestSystem/XXX/XXX.h` - [Description]

### Modified Files
- `Source/cplus/XXX/XXX.cpp` - [What was changed]

### Deleted Files
- `Source/cplus/XXX/OldFile.cpp` - [Why deleted]

---

## Handoff Notes

### Current Status
- [Describe what's complete and what remains]

### Next Priority
- [What should next developer focus on]

### Warnings
- [Things to be aware of]
- [Unreal Engine-specific issues]

---

## Session End

**End:** HH:MM
**Commits:** [number]
**Lines Changed:** +X / -Y
**Build Status:** [OK/FAIL]
**UE Version:** 5.7
```

---

## 12. Identify Today's Work

### Based on:
1. **ROADMAP** - What task is next in sequence?
2. **Issues** - Any assigned issues?
3. **Handoff** - What did previous developer leave?
4. **Blockers** - Any critical bugs to fix?
5. **Quest System** - Any quest functions to implement?

### Priority Order
1. 🔴 **Build Failures** - Not compiling
2. 🟠 **Quest System Bugs** - Critical quest issues
3. 🟡 **Current Sprint** - Planned tasks
4. 🟢 **Backlog** - Future tasks

---

## Session Start Checklist

### Mandatory
- [ ] Git status checked
- [ ] No critical uncommitted changes
- [ ] Build works (Development)
- [ ] Unreal Editor starts without crash
- [ ] C++ classes visible in Content Browser

### Recommended
- [ ] Previous session report read
- [ ] ROADMAP progress checked
- [ ] CHANGELOG reviewed
- [ ] Quest system status verified
- [ ] Today's session report created
- [ ] Today's goals identified

---

## ⚠️ IMPORTANT RULES

1. **NEVER git commit/push without explicit instruction**
2. **Document ALL significant changes**
3. **Run build after every major change**
4. **Update session report continuously**
5. **Use Live Coding for small .cpp changes**
6. **Restart Editor for .h changes**

---

## Quick Reference

### Unreal Engine Structure
```
Source/cplus/
├── QuestSystem/
│   ├── Core/           # QuestDefinition, QuestTask, QuestSubsystem
│   ├── Components/     # QuestTracker, QuestGiver, QuestTarget
│   ├── Tasks/          # Kill, Collect, Location tasks
│   └── Interfaces/     # QuestInteractable, QuestKillable
├── Variant_Shooter/    # Shooter game variant
└── Variant_Horror/     # Horror game variant
```

### Common Commands
```powershell
# Build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Start Editor
Start-Process "C:\ue5.7\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Live Coding (in Editor)
# CTRL+ALT+F11

# Git status
git status --short

# Recent commits
git log --oneline -5
```

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
