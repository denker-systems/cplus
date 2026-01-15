---
description: Keep documentation synchronized with development
auto_execution_mode: 1
---
# Update Documentation Workflow

## 1. Documentation Types

### Core Documentation
| File | Purpose | Update Frequency |
|-----|---------|-----------------|
| `CHANGELOG.md` | All changes | After every commit |
| `ROADMAP.md` | Project plan | Weekly |
| `README.md` | Overview | At major changes |

### Development Documentation
| File | Purpose | Update Frequency |
|-----|---------|-----------------|
| `sessions/YYYY-MM-DD.md` | Daily report | Every session |
| `DEVLOG_YYYY-MM.md` | Monthly log | Weekly |

## 2. CHANGELOG

**Format:**
```markdown
## [Unreleased]
### Added
- Feature (scope)
### Changed
- Change
### Fixed
- Bug fix (#123)
```

**Update:** feat→Added, fix→Fixed, refactor/chore→Changed

## 3. ROADMAP

**Format:**
```markdown
## v0.2.0 - Quest System (Current)
### Core
- [x] Completed
- [ ] In progress
### Integration
- [ ] Planned
```

**Update:** Mark [x] completed, update dates, add next sprint

## 4. Session Report

**Create:** `docs/dev/sessions/YYYY-MM-DD.md`

**Include:** Developer, branch, start, focus, goals, progress log, commits table, decisions, UE status, blockers, code changes, handoff notes, end summary

*See previous sessions for template*

## 5. DEVLOG

**Format:**
```markdown
# Devlog - Month YYYY

## Week N
### Focus: [Topic]
#### Done: [Work]
#### Learned: [Insights]
#### Problems/Solutions: [Issues]
#### Next: [Steps]

## Month Summary
### Accomplishments: [List]
### Challenges: [List]
### Next Month: [Goals]
```

## 6. API Docs

**Use Doxygen style:**
```cpp
/**
 * Class description
 * @see RelatedClass
 */
UCLASS()
class UMyClass {
    /** Method description
     * @param Param Description
     * @return Description
     */
    UFUNCTION()
    bool Method(Type* Param);
};
```

## 7. Git Sync

**Commit message:** Reference CHANGELOG, close issues (#123)

## 8. Templates

**Feature:** Overview, implementation, usage, examples  
**Bug Fix:** Problem, root cause, solution, testing

## 9. Quality

- [ ] Functions documented
- [ ] Logic explained
- [ ] Examples provided
- [ ] Formatted
- [ ] Spell checked

## 10. Automation

```powershell
# update-docs.ps1
param([string]$Type)
switch ($Type) {
    "changelog" { # Update CHANGELOG }
    "session" { # Create session }
    "devlog" { # Update devlog }
}
```

## Quick Commands

```powershell
$today = Get-Date -Format 'yyyy-MM-dd'
New-Item "docs/dev/sessions/$today.md"
```

**Structure:** `docs/CHANGELOG.md`, `docs/ROADMAP.md`, `docs/dev/sessions/`, `docs/dev/DEVLOG_YYYY-MM.md`

## Checklist

**Commit:** CHANGELOG, code comments  
**Session:** Session report, DEVLOG, handoff  
**Sprint:** ROADMAP, sprint review  
**Release:** CHANGELOG version, release notes, README

---

**v2.1 Ultra-optimized | 2026-01-14**
