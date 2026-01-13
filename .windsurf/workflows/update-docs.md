# Unreal Engine Update Documentation Workflow

> **Destination:** `.windsurf/workflows/update-docs.md`

---
description: Update project documentation after changes
auto_execution_mode: 1
---

# Update Documentation Workflow

> Keep documentation synchronized with development

## 1. Documentation Types

### Core Documentation
| Type | File | Purpose | Update Frequency |
|-----|-----|-------|-----------------|
| **CHANGELOG** | `docs/CHANGELOG.md` | All changes | After every commit |
| **ROADMAP** | `docs/ROADMAP.md` | Project plan | Weekly |
| **README** | `README.md` | Project overview | At major changes |
| **ARCHITECTURE** | `docs/ARCHITECTURE.md` | System architecture | At architecture changes |

### Development Documentation
| Type | File | Purpose | Update Frequency |
|-----|-----|-------|-----------------|
| **Session Report** | `docs/dev/sessions/YYYY-MM-DD.md` | Daily report | Every session |
| **DEVLOG** | `docs/dev/DEVLOG_YYYY-MM.md` | Monthly log | Weekly |
| **API Docs** | `docs/api/` | API reference | At API changes |
| **Guides** | `docs/guides/` | How-to guides | At new features |

---

## 2. CHANGELOG Update

### Format
```markdown
# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- New feature description (scope: quest/shooter/horror/ai/ui)
- Another new feature

### Changed
- Changed existing feature description

### Fixed
- Bug fix description (issue #123)

### Removed
- Removed feature description

## [1.2.0] - 2026-01-13

### Added
- Quest system implementation
- NPC interaction system

### Changed
- Updated AI behavior tree

### Fixed
- Fixed weapon reload timing

## [1.1.0] - 2026-01-10
...
```

### Update After Commit
```powershell
# 1. Read current CHANGELOG
$changelog = Get-Content "docs/CHANGELOG.md" -Raw

# 2. Extract latest commit info
$lastCommit = git log -1 --format="%s"
$commitType = ($lastCommit -split ':')[0]
$commitScope = ($lastCommit -split ':')[1] -split ' ' | Select-Object -First 1
$commitDesc = ($lastCommit -split ':')[1] -replace '^[^ ]+\s*', ''

# 3. Add to correct section
switch ($commitType) {
    "feat" { $section = "### Added" }
    "fix" { $section = "### Fixed" }
    "refactor" { $section = "### Changed" }
    "chore" { $section = "### Changed" }
    default { $section = "### Changed" }
}

# 4. Update file (implement logic)
```

---

## 3. ROADMAP Update

### Format
```markdown
# ROADMAP

## Current Status
- **Version:** v0.2.0 (In Development)
- **Focus:** Quest System
- **Target:** 2026-01-20

## v0.2.0 - Quest System (Current Sprint)
**Target:** 2026-01-20
**Status:** In Progress

### Core Features
- [x] Quest Definition DataAssets
- [x] Quest Tracker Component
- [x] Quest Giver Component
- [ ] Quest Target Component
- [ ] Quest Reward System

### Integration
- [ ] NPC Quest Integration
- [ ] Player Quest Journal UI
- [ ] Quest Progress Visualization

### Testing
- [ ] Unit Tests for Quest Logic
- [ ] Integration Tests
- [ ] Performance Tests

## v0.3.0 - AI Enhancement (Next Sprint)
**Target:** 2026-02-03
**Status:** Planned

### Features
- [ ] Advanced Behavior Trees
- [ ] AI Perception System
- [ ] Group AI Behavior

## v1.0.0 - First Release
**Target:** 2026-03-01
**Status:** Planned

### Features
- [ ] Complete Quest System
- [ ] Shooter Gameplay Loop
- [ ] Horror Gameplay Loop
- [ ] Save/Load System
- [ ] Settings Menu
```

### Update After Sprint
```powershell
# 1. Mark completed tasks
# 2. Move to "Completed" section
# 3. Update status and date
# 4. Add next sprint
```

---

## 4. Session Report

### Create New Session
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
- [ ] Implement QuestTrackerComponent
- [ ] Fix AI navigation bug

### Secondary Goals
- [ ] Update documentation
- [ ] Code review

---

## Progress Log

### 09:00 - Session Start
- Git status: Clean
- Branch: feature/quest-system
- Build status: OK
- Last commit: abc123 feat(quest): Add base quest classes

### 09:15 - Implement QuestTrackerComponent
- Created UQuestTrackerComponent class
- Added delegates for quest events
- Integrated with QuestSubsystem

### 11:30 - Lunch

### 12:00 - Fix AI Navigation
- Found bug in NavSystem init
- Added null check in AIController
- Tested with multiple NPCs

### 14:30 - Code Review
- Reviewed quest system code
- Fixed naming conventions
- Added documentation comments

---

## Commits This Session

| Time | Hash | Type | Scope | Description |
|-----|------|------|-------|-------------|
| 09:45 | `def456` | feat | quest | Add QuestTrackerComponent |
| 10:30 | `ghi789` | fix | ai | Fix NavSystem null reference |
| 13:15 | `jkl012` | refactor | quest | Improve quest event system |

---

## Technical Decisions

### Decision 1: Use Subsystem for Quest Management
- **Context:** Needed global access to quest data
- **Decision:** Use UGameInstanceSubsystem
- **Consequences:** Quest data persists between level changes

---

## Unreal Engine Specific

### Quest System
- [x] Status: Working
- [x] Last change: Added reward system
- [x] Next step: UI integration

### Build System
- [x] Build status: OK
- [x] Last build: 14:45
- [x] Known issues: None

---

## Blockers and Problems

| Problem | Status | Solution |
|---------|--------|---------|
| Live Coding fails | Resolved | Restarted editor |
| Memory leak in QuestSubsystem | Resolved | Used TObjectPtr |

---

## Code Changes

### New Files
- `Source/cplus/QuestSystem/Components/QuestTrackerComponent.h` - Track player quests
- `Source/cplus/QuestSystem/Components/QuestTrackerComponent.cpp` - Implementation

### Modified Files
- `Source/cplus/QuestSystem/Core/QuestSubsystem.h` - Added reward delegates
- `Source/cplus/QuestSystem/Core/QuestSubsystem.cpp` - Implemented reward logic

---

## Handoff Notes

### Current Status
- Quest system core is implemented
- AI navigation bug is fixed
- Everything builds and runs

### Next Priority
- Implement QuestTargetComponent
- Create UI for quest journal
- Start NPC integration

### Warnings
- Quest rewards need balancing
- Performance testing for many active quests

---

## Session End

**End:** 16:00
**Commits:** 3
**Lines Changed:** +245 / -12
**Build Status:** OK
**UE Version:** 5.7
```

---

## 5. DEVLOG Update

### Monthly Devlog Format
```markdown
# Devlog - January 2026

## Week 1 (2026-01-01 - 2026-01-07)
### Focus: Project Setup & Core Systems

#### What was done?
- Set up Unreal Engine 5.7 project
- Implemented basic Character classes
- Created project structure for multiple game variants

#### What did we learn?
- Unreal Engine's module system is powerful
- Live Coding works well for small changes
- Git ignore for Unreal projects is important

#### Problems & Solutions
- **Problem:** Build artifacts in git
  - **Solution:** Created comprehensive .gitignore

#### Next Steps
- Start quest system implementation

---

## Week 2 (2026-01-08 - 2026-01-14)
### Focus: Quest System

#### What was done?
- Implemented QuestSubsystem
- Created QuestTrackerComponent
- Added interfaces for quest interaction

#### What did we learn?
- Subsystems are perfect for global managers
- GameplayTags are powerful for categorization
- Component pattern works well in Unreal

#### Problems & Solutions
- **Problem:** UHT errors with struct pointers
  - **Solution:** Used const reference instead

#### Next Steps
- Quest UI implementation
- NPC quest integration

---

## Month Summary
### Accomplishments
- ✅ Project setup complete
- ✅ Core systems in place
- ✅ Quest system 60% complete

### Challenges
- Learning curve for Unreal C++
- Memory management with UPROPERTY
- Build system optimization

### February Goals
- 🎯 Complete quest system
- 🎯 Start UI system
- 🎯 Implement save/load
```

---

## 6. API Documentation

### Code Comments (Unreal Style)
```cpp
/**
 * Quest Tracker Component
 * 
 * This component tracks the player's active quests and broadcasts
 * quest-related events. It integrates with the QuestSubsystem
 * to manage quest state and progress.
 * 
 * @see UQuestSubsystem For quest management
 * @see UQuestDefinition For quest data structure
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API UQuestTrackerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /**
     * Accept a new quest and add it to the active quests list
     * 
     * @param Quest The quest definition to accept
     * @return True if quest was successfully accepted
     */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool AcceptQuest(UQuestDefinition* Quest);

    /**
     * Event broadcast when a quest is completed
     * 
     * @param QuestID The ID of the completed quest
     * @param Rewards The rewards granted for completion
     */
    UPROPERTY(BlueprintAssignable, Category = "Quest")
    FOnQuestCompleted OnQuestCompleted;
};
```

### Generate API Docs
```powershell
# Use Doxygen or Unreal's tool
# Documentation generated to docs/api/
```

---

## 7. Documentation Sync with Git

### Pre-commit Hook (optional)
```powershell
# .git/hooks/pre-commit
# Check that CHANGELOG is updated
$staged = git diff --cached --name-only
if ($staged -match "Source/") {
    if (-not (git diff --cached --name-only | Select-String "CHANGELOG")) {
        Write-Host "WARNING: Source code changed but CHANGELOG not updated!" -ForegroundColor Yellow
        # exit 1 # Uncomment to block commit
    }
}
```

### Commit Message References
```bash
feat(quest): Add quest tracker component

Implement component to track player's active quests.
Broadcasts events for quest progress and completion.

Closes #123
See CHANGELOG.md for details
```

---

## 8. Documentation Templates

### New Feature Template
```markdown
## [Feature Name]

### Overview
[Brief description of the feature]

### Implementation
[Technical details]

### Usage
[How to use it]

### Examples
[Code examples]

### API Reference
[Link to API docs]
```

### Bug Fix Template
```markdown
## Bug Fix: [Title]

### Problem
[Description of the bug]

### Root Cause
[What caused it]

### Solution
[How it was fixed]

### Testing
[How it was tested]
```

---

## 9. Documentation Quality

### Checklist
- [ ] All public functions documented
- [ ] Complex logic explained
- [ ] Examples provided
- [ ] API references linked
- [ ] Screenshots where helpful
- [ ] Code blocks formatted
- [ ] Spelling and grammar checked

---

## 10. Automation Tools

### PowerShell Script for Auto-Update
```powershell
# update-docs.ps1
param(
    [string]$Type = "changelog",
    [string]$Message = ""
)

switch ($Type) {
    "changelog" {
        # Update CHANGELOG with latest commit
        $lastCommit = git log -1 --format="%s"
        # ... implement logic
    }
    "session" {
        # Create session report
        # ... implement logic
    }
    "devlog" {
        # Update devlog
        # ... implement logic
    }
}

Write-Host "Documentation updated!" -ForegroundColor Green
```

---

## Quick Reference

### Common Commands
```powershell
# Update CHANGELOG
# Add changes under ## [Unreleased]

# Create session report
$today = Get-Date -Format 'yyyy-MM-dd'
New-Item "docs/dev/sessions/$today.md"

# Update ROADMAP
# Mark [x] for completed tasks

# Generate API docs
# Use Unreal's documentation tool
```

### Documentation Structure
```
docs/
├── CHANGELOG.md              # All changes
├── ROADMAP.md                # Project plan
├── README.md                 # Project overview
├── ARCHITECTURE.md           # System design
├── api/                      # API reference
│   ├── quest-system.md
│   └── ai-system.md
├── guides/                   # How-to guides
│   ├── getting-started.md
│   └── quest-creation.md
└── dev/                      # Development docs
    ├── sessions/             # Daily reports
    │   └── YYYY-MM-DD.md
    └── DEVLOG_YYYY-MM.md     # Monthly logs
```

---

## Documentation Update Checklist

### After Every Commit
- [ ] CHANGELOG updated
- [ ] Relevant API docs updated
- [ ] Code comments added/updated

### After Every Session
- [ ] Session report created
- [ ] DEVLOG updated
- [ ] Handoff notes written

### After Every Sprint
- [ ] ROADMAP updated
- [ ] Weekly devlog written
- [ ] Sprint review documented

### After Every Release
- [ ] CHANGELOG versioned
- [ ] Release notes created
- [ ] API docs updated
- [ ] README updated

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
