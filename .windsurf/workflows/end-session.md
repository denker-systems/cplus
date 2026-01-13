# Unreal Engine End Session Workflow

> **Destination:** `.windsurf/workflows/end-session.md`

---
description: End development session with full documentation sync
auto_execution_mode: 1
---

# End Session Workflow

> End development session with full documentation synchronization

## 1. Session Status Check

// turbo
```powershell
$endTime = Get-Date -Format 'yyyy-MM-dd HH:mm'
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "=== UNREAL ENGINE SESSION END: $endTime ===" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
```

---

## 2. Git Status - Check Changes

// turbo
```powershell
Write-Host "`n=== GIT STATUS ===" -ForegroundColor Yellow
git status --short

# Show changed files
$changedFiles = git diff --name-only
Write-Host "`nChanged files: $($changedFiles.Count)" -ForegroundColor Green
foreach ($file in $changedFiles) {
    Write-Host "  - $file" -ForegroundColor Gray
}
```

### Analyze
- [ ] Uncommitted changes?
- [ ] New files not in git?
- [ ] Staged changes?
- [ ] Merge conflicts?

---

## 3. Commits This Session

// turbo
```powershell
Write-Host "`n=== COMMITS THIS SESSION ===" -ForegroundColor Yellow
git log --oneline --since="1 day ago" --format="%h %s" | Select-Object -First 10

# Get commit count
$commitCount = git rev-list --count HEAD ^@{yesterday}
Write-Host "Commits today: $commitCount" -ForegroundColor Green
```

---

## 4. Build Status - Final Verification

// turbo
```powershell
Write-Host "`n=== FINAL BUILD CHECK ===" -ForegroundColor Yellow
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -WaitMutex 2>&1 | Select-Object -Last 5

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build OK" -ForegroundColor Green
} else {
    Write-Host "❌ BUILD FAILED - Fix before closing!" -ForegroundColor Red
}
```

---

## 5. Update Session Report

### Find Today's Session
```powershell
$today = Get-Date -Format 'yyyy-MM-dd'
$sessionFile = "docs/dev/sessions/$today.md"

if (Test-Path $sessionFile) {
    Write-Host "Found session: $sessionFile" -ForegroundColor Green
} else {
    Write-Host "Creating new session: $sessionFile" -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "docs/dev/sessions" -Force -ErrorAction SilentlyContinue
}
```

### Update End Section
```markdown
## Session End

**End:** HH:MM
**Commits:** [number]
**Lines Changed:** +X / -Y
**Build Status:** [OK/FAIL]
**UE Version:** 5.7

---

## Handoff Notes

### Current Status
- [Describe what was completed and what remains]

### Next Priority
- [What should next developer focus on]

### Warnings
- [Things to be aware of]
- [Unreal Engine-specific problems]

---

## Technical Decisions

### Decision 1: [Title]
- **Context:** Why was decision needed?
- **Decision:** What was decided?
- **Consequences:** Impact on system

---

## Next Steps

1. [ ] Fix build if it fails
2. [ ] Commit all changes
3. [ ] Update CHANGELOG
4. [ ] Update ROADMAP
5. [ ] Create handoff notes
```

---

## 6. CHANGELOG Update

### Find Unreleased Section
```powershell
Write-Host "`n=== UPDATE CHANGELOG ===" -ForegroundColor Yellow
$changelog = Get-Content "docs/CHANGELOG.md" -Raw

# Check if Unreleased exists
if ($changelog -match "## \[Unreleased\]") {
    Write-Host "Unreleased section exists" -ForegroundColor Green
} else {
    Write-Host "Adding Unreleased section" -ForegroundColor Yellow
    $changelog = "## [Unreleased]`n`n$changelog"
    Set-Content "docs/CHANGELOG.md" -Value $changelog
}
```

### Add Today's Changes
```markdown
## [Unreleased]

### Added
- [Feature 1] - [Description]
- [Feature 2] - [Description]

### Changed
- [Changed 1] - [Description]

### Fixed
- [Fix 1] - [Description]
- [Fix 2] - [Description]

### Removed
- [Removed 1] - [Description]
```

---

## 7. ROADMAP Update

### Mark Completed Tasks
```powershell
Write-Host "`n=== UPDATE ROADMAP ===" -ForegroundColor Yellow
# Show current ROADMAP
Get-Content "docs/ROADMAP.md" | Select-Object -First 50
```

### Update Progress
```markdown
### Current Sprint (v0.2.0)
- [x] [Completed task 1]
- [x] [Completed task 2]
- [ ] [In progress task]

### Next Sprint
- [ ] Next task 1
- [ ] Next task 2
```

---

## 8. DEVLOG Update

### Create Today's Devlog
```powershell
$devlogFile = "docs/dev/DEVLOG_$(Get-Date -Format 'yyyy-MM').md"
$today = Get-Date -Format 'yyyy-MM-dd'
```

### Devlog Entry
```markdown
# Devlog - $(Get-Date -Format 'MMMM yyyy')

## $today - [Session's main focus]

### What was done?
- [Description of work]
- [Description of work]

### What did we learn?
- [Insight 1]
- [Insight 2]

### Problems & Solutions
- **Problem:** [Description]
  - **Solution:** [How it was solved]

### Next Steps
- [Priority 1]
- [Priority 2]

---
```

---

## 9. Code Review Checklist

### Before Session End
```powershell
Write-Host "`n=== CODE REVIEW CHECKLIST ===" -ForegroundColor Yellow
$checklist = @(
    "All code compiles (Development)",
    "No build artifacts in git",
    "UPROPERTY/UFUNCTION correct",
    "Memory management OK (TObjectPtr)",
    "No unnecessary ticks",
    "Error handling with check/ensure",
    "Documentation updated",
    "CHANGELOG updated"
)

foreach ($item in $checklist) {
    Write-Host "□ $item" -ForegroundColor Gray
}
```

---

## 10. Final Git Operations

### Staged Changes?
```powershell
$staged = git diff --cached --name-only
if ($staged) {
    Write-Host "⚠️ WARNING: Staged changes exist!" -ForegroundColor Yellow
    Write-Host "Use 'git commit' to commit or 'git reset' to unstage"
}
```

### Uncommitted Changes?
```powershell
$uncommitted = git diff --name-only
if ($uncommitted) {
    Write-Host "⚠️ WARNING: Uncommitted changes exist!" -ForegroundColor Yellow
    Write-Host "You should commit these before session ends:"
    foreach ($file in $uncommitted) {
        Write-Host "  - $file" -ForegroundColor Gray
    }
}
```

---

## 11. Clean Up

### Clean Temporary Files
```powershell
Write-Host "`n=== CLEANUP ===" -ForegroundColor Yellow
# Remove temporary files
Remove-Item "*.tmp" -Force -ErrorAction SilentlyContinue
Remove-Item "*.bak" -Force -ErrorAction SilentlyContinue

# Clean Unreal temp (optional)
# Remove-Item "Saved\Logs\*.log" -Force -ErrorAction SilentlyContinue
```

---

## 12. Session Summary

### Generate Report
```powershell
Write-Host "`n=== SESSION SUMMARY ===" -ForegroundColor Green
Write-Host "Session start: [Start time]"
Write-Host "Session end: $endTime"
Write-Host "Total time: [Calculate time]"
Write-Host "Commits: $commitCount"
Write-Host "Build status: $(if ($LASTEXITCODE -eq 0) { 'OK' } else { 'FAIL' })"
Write-Host "UE Version: 5.7"
```

### Email/Slack Summary (optional)
```markdown
Unreal Engine Session Summary
=============================
Date: $today
Developer: $env:USERNAME
Commits: $commitCount
Build: OK/FAIL

Completed:
- [Task 1]
- [Task 2]

Next:
- [Priority 1]
- [Priority 2]

Blockers:
- [Any blockers]
```

---

## 13. Documentation Sync Status

### Check All is Synced
```powershell
Write-Host "`n=== DOCUMENTATION SYNC ===" -ForegroundColor Yellow
$docs = @(
    "docs/dev/sessions/$today.md",
    "docs/CHANGELOG.md",
    "docs/ROADMAP.md",
    "docs/dev/DEVLOG_$(Get-Date -Format 'yyyy-MM').md"
)

foreach ($doc in $docs) {
    if (Test-Path $doc) {
        Write-Host "✅ $doc" -ForegroundColor Green
    } else {
        Write-Host "❌ $doc" -ForegroundColor Red
    }
}
```

---

## 14. Session End Checklist

### Mandatory
- [ ] All changes committed
- [ ] Build works
- [ ] Session report updated
- [ ] CHANGELOG updated
- [ ] ROADMAP updated
- [ ] DEVLOG updated

### Recommended
- [ ] Code review performed
- [ ] Handoff notes written
- [ ] Temporary files removed
- [ ] Workspace cleaned
- [ ] Next session planned

---

## 15. Automated Session End

### PowerShell Script
```powershell
# Save as end-session.ps1
param(
    [string]$Summary = "Session completed"
)

# 1. Update session report
$today = Get-Date -Format 'yyyy-MM-dd'
$sessionFile = "docs/dev/sessions/$today.md"

# 2. Add end section
Add-Content $sessionFile @"

## Session End

**End:** $(Get-Date -Format 'HH:mm')
**Build Status:** $(if ($LASTEXITCODE -eq 0) { 'OK' } else { 'FAIL' })
**UE Version:** 5.7

---

$Summary
"@

# 3. Update CHANGELOG
# (add logic to update)

Write-Host "✅ Session ended and documented!" -ForegroundColor Green
```

---

## ⚠️ IMPORTANT RULES AT SESSION END

1. **NEVER close editor with uncommitted changes**
2. **ALWAYS verify build works**
3. **ALWAYS update documentation**
4. **NEVER leave workspace in broken state**
5. **ALWAYS write handoff notes for next developer**

---

## Quick Reference

### Common Commands
```powershell
# Git status
git status --short

# Show commits today
git log --oneline --since="1 day ago"

# Build check
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Update CHANGELOG
# Add changes under ## [Unreleased]

# Find session report
$today = Get-Date -Format 'yyyy-MM-dd'
$sessionFile = "docs/dev/sessions/$today.md"
```

### Documentation Structure
```
docs/
├── dev/
│   ├── sessions/
│   │   └── YYYY-MM-DD.md          # Session report
│   └── DEVLOG_YYYY-MM.md          # Monthly devlog
├── CHANGELOG.md                   # Project changelog
└── ROADMAP.md                     # Project roadmap
```

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
