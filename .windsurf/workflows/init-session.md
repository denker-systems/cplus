---
description: Start UE 5.7 development session
auto_execution_mode: 3
---

# Init Session Workflow

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

---

## 3. Recent Commits - Understand Context

// turbo
```powershell
Write-Host "`n=== LAST 10 COMMITS ===" -ForegroundColor Yellow
git log --oneline -10 --format="%h %an: %s (%ar)"
```

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

## 7. Previous Session

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

---

## 8. ROADMAP

// turbo
```powershell
Write-Host "`n=== ROADMAP ===" -ForegroundColor Yellow
Get-Content docs/ROADMAP.md -ErrorAction SilentlyContinue | Select-Object -First 60
```

---

## 9. CHANGELOG

// turbo
```powershell
Write-Host "`n=== CHANGELOG (Unreleased) ===" -ForegroundColor Yellow
Get-Content docs/CHANGELOG.md -ErrorAction SilentlyContinue | Select-Object -First 40
```

---

## 10. Start Unreal Editor

```powershell
Start-Process "C:\ue5.7\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" -ArgumentList "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

---

## 11. Create Session Report

Create `docs/dev/sessions/YYYY-MM-DD.md` with:
- Developer, branch, start time, focus
- Session goals (primary/secondary)
- Progress log with timestamps
- Commits table
- Technical decisions
- UE-specific status (Quest System, Build)
- Blockers/problems
- Code changes (new/modified/deleted)
- Handoff notes

*See previous session reports for template*

---

## 12. Identify Work Priority

1. 🔴 Build failures
2. 🟠 Critical bugs
3. 🟡 ROADMAP tasks
4. 🟢 Backlog

---

## Checklist

- [ ] Git status clean
- [ ] Build successful
- [ ] Editor starts
- [ ] Session report created
- [ ] Goals identified

---

---

**Version:** 2.0 (Optimized for context efficiency)  
**Last Updated:** 2026-01-14
