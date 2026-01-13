# Unreal Engine Build Workflow

> **Destination:** `.windsurf/workflows/build.md`

---
description: Complete build process for Unreal Engine 5.7 C++
auto_execution_mode: 1
---

# Build Workflow

> Build and compile Unreal Engine C++ project

## 1. Quick Build (Live Coding)

### Hot Reload - .cpp changes only
// turbo
```powershell
# CTRL+ALT+F11 in Unreal Editor
# OR
Write-Host "=== LIVE CODING BUILD ===" -ForegroundColor Cyan
# Click "Compile" in Unreal Editor (bottom right)
```

**Limitations:**
- ✅ Can change .cpp implementation
- ❌ CANNOT add new UCLASS, UPROPERTY, UFUNCTION
- ❌ CANNOT change .h files
- ❌ CANNOT add new files

---

## 2. Full Build - Editor Restart

### When .h files are changed
```powershell
# 1. Close Unreal Editor
# 2. Open project again (double-click cplus.uproject)
# 3. Editor compiles automatically
```

**Use when:**
- New classes created
- UPROPERTY/UFUNCTION added
- Header files changed
- New modules added

---

## 3. Build via Unreal Build Tool

### Standard Build
// turbo
```powershell
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -WaitMutex
```

### Build Configurations

| Config | Optimization | Debug Info | Usage |
|--------|-------------|------------|------------|
| **DebugGame** | None | Full | Development with breakpoints |
| **Development** | Some | Partial | Daily development |
| **Shipping** | Full | None | Final release |
| **Test** | Full | Some | QA testing |

### Build Specific Configuration
```powershell
# Debug build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 DebugGame "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Shipping build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplus Win64 Shipping "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

---

## 4. Build via Visual Studio

### Setup
```powershell
# Generate VS project files
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -game -rocket -progress
```

### Build in Visual Studio
1. Open `cplus.sln`
2. Set build configuration (Development Editor)
3. Press **Ctrl+Shift+B** (Build Solution)
4. Or right-click project → Build

---

## 5. Clean Build

### Full Rebuild
```powershell
# Delete Intermediate and Binaries
Remove-Item -Recurse -Force "Intermediate" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "Binaries" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "Saved\Logs" -ErrorAction SilentlyContinue

# Regenerate project files
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -game -rocket

# Build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

---

## 6. Build Errors

### Common Compiler Errors

| Error | Cause | Solution |
|-------|-------|---------|
| `error C2065: undeclared identifier` | Missing include or forward decl | Add #include |
| `error C2039: is not a member` | Wrong class/method | Check spelling |
| `error C2664: cannot convert` | Type mismatch | Check types |
| `error C2248: cannot access` | Private/protected member | Change access or use getter |
| `LINK error LNK2019` | Missing implementation | Implement method in .cpp |

### Common UHT Errors

| Error | Cause | Solution |
|-------|-------|---------|
| `Inappropriate '*' on variable` | Exposed pointer to struct | Remove UFUNCTION or use const ref |
| `GENERATED_BODY() not found` | Missing macro | Add GENERATED_BODY() |
| `Unknown type` | Missing #include | Include header before .generated.h |
| `Multiple definitions` | Missing #pragma once | Add #pragma once |

### Debugging Build Errors
```powershell
# Find errors
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" 2>&1 | Select-String "error"

# Find warnings
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" 2>&1 | Select-String "warning"
```

---

## 7. Build Targets

### Available Targets

| Target | Output | Description |
|--------|--------|-------------|
| **cplus** | Game executable | Standalone game |
| **cplusEditor** | Editor DLL | For Unreal Editor |
| **cplusServer** | Server executable | Dedicated server |

### Build Specific Target
```powershell
# Editor (most common)
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Standalone game
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplus Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

---

## 8. Build Verification

### Check Build Success
```powershell
if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ BUILD SUCCESSFUL" -ForegroundColor Green
} else {
    Write-Host "❌ BUILD FAILED" -ForegroundColor Red
}
```

### Verify DLL Created
```powershell
if (Test-Path "Binaries\Win64\UnrealEditor-cplus.dll") {
    Write-Host "✅ DLL created successfully" -ForegroundColor Green
} else {
    Write-Host "❌ DLL not found" -ForegroundColor Red
}
```

---

## 9. Build Checklist

### Before Build
- [ ] All .h files saved
- [ ] All .cpp files saved
- [ ] No syntax errors in IDE
- [ ] .Build.cs updated if new dependencies

### After Build
- [ ] Build succeeded (exit code 0)
- [ ] No new warnings
- [ ] DLL created in Binaries/Win64/
- [ ] Editor starts without crash

---

## 10. Workflow Decision Tree

```
Change made?
│
├─ Only .cpp changed?
│  └─ Use Live Coding (CTRL+ALT+F11)
│
├─ .h file changed?
│  └─ Restart Editor
│
├─ New class created?
│  └─ Restart Editor
│
├─ .Build.cs changed?
│  └─ Full rebuild (delete Intermediate/)
│
└─ Strange errors?
   └─ Clean build (delete Intermediate/ and Binaries/)
```

---

## Quick Reference

### Most Common Commands

```powershell
# Live Coding (cpp only)
# CTRL+ALT+F11 in Editor

# Full build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Clean build
Remove-Item -Recurse -Force "Intermediate","Binaries" -ErrorAction SilentlyContinue
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Regenerate project files
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject" -game -rocket
```

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
