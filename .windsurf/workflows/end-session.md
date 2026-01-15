---
description: End UE 5.7 session with documentation sync
auto_execution_mode: 3
---
# End Session Workflow



## 1. Status Check

// turbo
```powershell
$endTime = Get-Date -Format 'yyyy-MM-dd HH:mm'
Write-Host "=== SESSION END: $endTime ===" -ForegroundColor Cyan

git status --short
$commitCount = (git log --oneline --since="1 day ago" | Measure-Object).Count
Write-Host "Commits today: $commitCount"
```

## 2. Build Check

// turbo
```powershell
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -WaitMutex 2>&1 | Select-Object -Last 5
if ($LASTEXITCODE -eq 0) { Write-Host "✅ Build OK" -ForegroundColor Green } else { Write-Host "❌ FIX BUILD!" -ForegroundColor Red }
```

## 3. Update Session Report

**Add to** `docs/dev/sessions/YYYY-MM-DD.md`:
- End time, commits, build status, handoff notes

## 4. Update CHANGELOG

Add under `## [Unreleased]`: Added/Changed/Fixed items from today.

## 5. Update ROADMAP

Mark completed tasks `[x]` in current sprint.

## 6. Update DEVLOG

Add entry: What done, problems/solutions, next steps.

## 7. Final Checks

```powershell
# Git check
git diff --name-only  # Uncommitted?
git diff --cached --name-only  # Staged?

# Clean up
Remove-Item "*.tmp","*.bak" -Force -ErrorAction SilentlyContinue

# Verify docs
$today = Get-Date -Format 'yyyy-MM-dd'
Test-Path "docs/dev/sessions/$today.md","docs/CHANGELOG.md","docs/ROADMAP.md"
```

## Checklist

- [ ] All committed
- [ ] Build OK
- [ ] Session report updated
- [ ] CHANGELOG/ROADMAP/DEVLOG updated
- [ ] Handoff notes written

**⚠️ NEVER close with uncommitted changes or broken build!**

---

**v2.1 Ultra-optimized | 2026-01-14**
