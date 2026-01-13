# Development Documentation

This directory contains development-specific documentation including session reports and development logs.

## Structure

```
docs/dev/
├── sessions/          # Daily session reports
│   ├── 2026-01-13.md  # Session report for Jan 13, 2026
│   └── 2026-01-14.md  # Session report for Jan 14, 2026
├── DEVLOG_2026-01.md  # Monthly development log for January 2026
└── README.md          # This file
```

## Session Reports

Session reports are created daily and track:
- Work completed during the session
- Commits made
- Technical decisions
- Blockers and problems
- Handoff notes for next developer

### Naming Convention
- Format: `YYYY-MM-DD.md`
- Example: `2026-01-13.md`

### Session Report Template
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

## Development Logs (DEVLOG)

Monthly development logs track:
- Weekly progress summaries
- What was learned
- Problems and solutions
- Next steps and goals

### Naming Convention
- Format: `DEVLOG_YYYY-MM.md`
- Example: `DEVLOG_2026-01.md`

### DEVLOG Template
```markdown
# Devlog - January 2026

## Week 1 (2026-01-01 - 2026-01-07)
### Focus: [Main focus of the week]

#### What was done?
- [Task 1]
- [Task 2]

#### What did we learn?
- [Insight 1]
- [Insight 2]

#### Problems & Solutions
- **Problem:** [Description]
  - **Solution:** [How it was solved]

#### Next Steps
- [Priority 1]
- [Priority 2]

---

## Week 2 (2026-01-08 - 2026-01-14)
### Focus: [Main focus of the week]
...

---

## Month Summary
### Accomplishments
- ✅ [Achievement 1]
- ✅ [Achievement 2]

### Challenges
- [Challenge 1]
- [Challenge 2]

### February Goals
- 🎯 [Goal 1]
- 🎯 [Goal 2]
```

## Best Practices

1. **Create session report at start of each day**
2. **Update continuously throughout the session**
3. **Document technical decisions with reasoning**
4. **Include handoff notes for team collaboration**
5. **Update monthly DEVLOG weekly**
6. **Keep all documentation in English**
7. **Link related files and resources**

## Automation

### PowerShell Script to Create Session
```powershell
# create-session.ps1
$today = Get-Date -Format 'yyyy-MM-dd'
$sessionFile = "docs/dev/sessions/$today.md"

# Create from template
# (Implementation details)
```

---

**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
