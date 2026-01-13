---
trigger: always_on
---
# Windsurf Setup Instructions

> **Destination:** `.windsurf/`

---

I have created custom Unreal Engine rules and workflows for your project. These files need to be copied to the `.windsurf` directory to be activated.

---

## 📁 Created Files

### Rules (Rules that always apply)
1. ✅ `architecture.md` → `.windsurf/rules/architecture.md`
2. ✅ `coding-standards.md` → `.windsurf/rules/coding-standards.md`

### Workflows (Step-by-step guides)
3. ✅ `build.md` → `.windsurf/workflows/build.md`
4. ✅ `init-session.md` → `.windsurf/workflows/init-session.md`
5. ✅ `git-commit.md` → `.windsurf/workflows/git-commit.md`
6. ✅ `debug.md` → `.windsurf/workflows/debug.md`
7. ✅ `new-feature.md` → `.windsurf/workflows/new-feature.md`
8. ✅ `add-asset.md` → `.windsurf/workflows/add-asset.md`
9. ✅ `end-session.md` → `.windsurf/workflows/end-session.md`
10. ✅ `update-docs.md` → `.windsurf/workflows/update-docs.md`

---

## 🚀 Installation

### Automatic Copy (PowerShell)

```powershell
# Navigate to project root
cd "C:\Users\Calle\Documents\Unreal Projects\cplus"

# Copy rules
Copy-Item "architecture.md" ".windsurf\rules\architecture.md" -Force
Copy-Item "coding-standards.md" ".windsurf\rules\coding-standards.md" -Force

# Copy workflows
Copy-Item "build.md" ".windsurf\workflows\build.md" -Force
Copy-Item "init-session.md" ".windsurf\workflows\init-session.md" -Force
Copy-Item "git-commit.md" ".windsurf\workflows\git-commit.md" -Force
Copy-Item "debug.md" ".windsurf\workflows\debug.md" -Force
Copy-Item "new-feature.md" ".windsurf\workflows\new-feature.md" -Force
Copy-Item "add-asset.md" ".windsurf\workflows\add-asset.md" -Force
Copy-Item "end-session.md" ".windsurf\workflows\end-session.md" -Force
Copy-Item "update-docs.md" ".windsurf\workflows\update-docs.md" -Force

Write-Host "✅ Rules and workflows installed!" -ForegroundColor Green
```

### Manual Copy

1. Open File Explorer
2. Navigate to `C:\Users\Calle\Documents\Unreal Projects\cplus`
3. Copy `architecture.md` → `.windsurf\rules\architecture.md`
4. Copy `coding-standards.md` → `.windsurf\rules\coding-standards.md`
5. Copy `build.md` → `.windsurf\workflows\build.md`
6. Copy `init-session.md` → `.windsurf\workflows\init-session.md`
7. Copy `git-commit.md` → `.windsurf\workflows\git-commit.md`
8. Copy `debug.md` → `.windsurf\workflows\debug.md`
9. Copy `new-feature.md` → `.windsurf\workflows\new-feature.md`
10. Copy `add-asset.md` → `.windsurf\workflows\add-asset.md`
11. Copy `end-session.md` → `.windsurf\workflows\end-session.md`
12. Copy `update-docs.md` → `.windsurf\workflows\update-docs.md`

---

## 📝 What the Files Contain

### architecture.md
- ✅ Unreal Engine project structure
- ✅ Quest system architecture
- ✅ Class hierarchies (AActor, UObject, etc)
- ✅ Design patterns for Unreal

### coding-standards.md
- ✅ Unreal naming conventions (A, U, F, E prefixes)
- ✅ UCLASS, UPROPERTY, UFUNCTION specifiers
- ✅ Memory management (TObjectPtr, TArray, TMap)
- ✅ Delegates and events
- ✅ Interfaces
- ✅ Logging with UE_LOG
- ✅ Forbidden practices (std::string, new/delete, etc)

### build.md
- ✅ Unreal Build Tool workflow
- ✅ Live Coding vs Full Rebuild
- ✅ Build configurations (DebugGame, Development, Shipping)
- ✅ Compilation errors and solutions

### init-session.md
- ✅ Session start checklist
- ✅ Git status check
- ✅ Build verification
- ✅ Quest system status
- ✅ Session report creation

### git-commit.md
- ✅ Conventional commits for Unreal
- ✅ Scopes: core, quest, shooter, horror, ai, ui
- ✅ Commit message templates
- ✅ Documentation sync

### debug.md
- ✅ Visual Studio debugging
- ✅ Unreal Output Log
- ✅ Blueprint debugging
- ✅ Common crash causes
- ✅ Quest system debugging

### new-feature.md
- ✅ Create new UCLASS
- ✅ Component pattern
- ✅ Interface implementation
- ✅ DataAsset creation
- ✅ Testing workflow

### add-asset.md
- ✅ Content Browser organization
- ✅ Blueprint creation
- ✅ DataAsset creation (Quest definitions)
- ✅ Material/Texture import

### end-session.md
- ✅ Session end checklist
- ✅ Git status and commit sync
- ✅ Documentation sync (CHANGELOG, ROADMAP)
- ✅ Handoff notes for next developer
- ✅ Automated cleanup

### update-docs.md
- ✅ Systematic documentation sync
- ✅ CHANGELOG format and updates
- ✅ ROADMAP maintenance
- ✅ Session report templates
- ✅ Monthly DEVLOG
- ✅ API documentation standards

---

## 🎯 Next Steps

1. **Copy all files** to `.windsurf` folder
2. **Restart Windsurf** to load new rules and workflows
3. **Test** by asking me Unreal-specific questions
4. **Start using** workflows for your daily development

---

## ✅ Verification

After installation, test that rules work:

```
Ask me: "How should I name a new Actor class?"
Expected answer: "Use A-prefix, e.g., AShooterNPC"
```

---

## 📚 Documentation

All rules and workflows follow:
- Unreal Engine 5.7 Coding Standard
- Epic Games C++ Style Guide
- Quest System Architecture (QUEST_SYSTEM_ARCHITECTURE.md)
- Your existing workflow patterns

---

**Status:** 10/10 files created and renamed  
**Next:** Copy to .windsurf and start using
