---
auto_execution_mode: 3
description: Research UE solutions across documentation and community
---

# Research Workflow

## 1. Define Scope

**Ask:** What solving? UE 5.7? Which system? Tried what? Errors?

## 2. Official Docs

**Primary:** https://docs.unrealengine.com/  
**API:** https://docs.unrealengine.com/5.7/en-US/API/  
**Learning:** https://dev.epicgames.com/community/learning

**Search:** "UGameInstanceSubsystem C++ example", "UPROPERTY specifiers", etc.

## 3. Community

**Forums:** https://forums.unrealengine.com/ (filter C++, "Solved")  
**Stack Overflow:** `site:stackoverflow.com UE5 C++ [topic]`  
**Reddit:** `site:reddit.com/r/unrealengine [topic]`

## 4. GitHub

**Search:**
```
repo:EpicGames/UnrealEngine [class]
language:C++ unreal engine [topic]
"UGameInstanceSubsystem" language:C++
```

**Samples:** ActionRPG, Lyra Starter Game

## 5. Videos

**Channels:** Unreal Engine, CodeLikeMe, Ryan Laley, Matt Aspland  
**Search:** `"UE5 C++ tutorial" [topic]`

## 6. Blogs

**Search:** `site:dev.to unreal engine [topic]`  
**Wiki:** https://unrealcommunity.wiki/  
**Tom Looman:** https://www.tomlooman.com/

## 7. Document Findings

**Create:** `docs/research/YYYY-MM-DD-[topic].md`

**Include:** Problem, sources, approaches (pros/cons), recommended solution, implementation plan, references

## 8. Checklist

**Before:** Define problem, UE 5.7, system, what tried  
**During:** Docs, forums, Stack Overflow, GitHub, videos  
**After:** Summarize, compare, select solution, plan, document

## 9. Common Queries

**Quest:** "UE5 quest system", "subsystem best practices", "GameplayTags"  
**AI:** "UE5 AI controller", "State Tree vs Behavior Tree", "AI perception"  
**Performance:** "UE performance optimization", "Tick vs event-driven"

## 10. Search Operators

`"exact phrase"`, `site:docs.unrealengine.com`, `-exclude`, `filetype:pdf`, `after:2024`

## 11. Best Practices

**DO:** Official docs first, verify multiple sources, check date, test, document  
**DON'T:** Trust single source, use outdated UE4, copy without understanding

## Quick Reference

**URLs:** docs.unrealengine.com, forums.unrealengine.com, unrealcommunity.wiki  
**Search:** `"UE5 C++ [topic]"`, `site:stackoverflow.com UE5 [q]`, `language:C++ "Unreal Engine" [topic]`

```powershell
$today = Get-Date -Format 'yyyy-MM-dd'
New-Item "docs/research/$today-[topic].md" -ItemType File -Force
```

---

**v2.1 Ultra-optimized | 2026-01-14**
