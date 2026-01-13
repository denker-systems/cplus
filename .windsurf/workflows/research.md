# Unreal Engine Research Workflow

> **Destination:** `.windsurf/workflows/research.md`

---
description: Research Unreal Engine solutions across documentation and community platforms
auto_execution_mode: 1
---

# Research Workflow

> Systematic research of Unreal Engine documentation and community resources

## 1. Define Research Scope

### Identify Research Need
```powershell
Write-Host "=== RESEARCH SCOPE ===" -ForegroundColor Cyan
Write-Host "What are you trying to solve or learn?"
Write-Host "- Feature implementation?"
Write-Host "- Bug fix?"
Write-Host "- Best practices?"
Write-Host "- Performance optimization?"
Write-Host "- Architecture pattern?"
```

### Research Questions
- [ ] What is the specific problem or question?
- [ ] What Unreal Engine version? (5.7 for this project)
- [ ] Which system is involved? (Quest, AI, UI, etc.)
- [ ] What have you already tried?
- [ ] Are there error messages or specific symptoms?

---

## 2. Official Unreal Engine Documentation

### Epic Games Documentation
**Primary Source:** https://docs.unrealengine.com/

#### Search Strategy:
1. **Unreal Engine 5.7 Docs**
   - Search: "Unreal Engine 5.7 [your topic]"
   - Focus on: C++ API, Gameplay Framework, Subsystems

2. **Key Documentation Sections:**
   - **C++ API Reference:** https://docs.unrealengine.com/5.7/en-US/API/
   - **Gameplay Framework:** https://docs.unrealengine.com/5.7/en-US/gameplay-framework-in-unreal-engine/
   - **Subsystems:** https://docs.unrealengine.com/5.7/en-US/programming-subsystems-in-unreal-engine/
   - **Components:** https://docs.unrealengine.com/5.7/en-US/components-in-unreal-engine/
   - **GameplayTags:** https://docs.unrealengine.com/5.7/en-US/using-gameplay-tags-in-unreal-engine/
   - **Delegates:** https://docs.unrealengine.com/5.7/en-US/delegates-and-lamba-functions-in-unreal-engine/

3. **Search Examples:**
   - "UGameInstanceSubsystem C++ example"
   - "UActorComponent best practices"
   - "UPROPERTY specifiers reference"
   - "Delegate broadcasting Unreal Engine"

### Epic Games Learning Portal
**URL:** https://dev.epicgames.com/community/learning

- Video tutorials
- Sample projects
- Community content
- Official courses

---

## 3. Community Forums and Q&A

### Unreal Engine Forums
**URL:** https://forums.unrealengine.com/

**Search Strategy:**
- Use specific keywords
- Filter by C++ category
- Look for "Solved" threads
- Check recent posts (UE5 specific)

**Example Searches:**
- "Quest system component C++"
- "Subsystem initialization order"
- "GameplayTags in C++ tutorial"

### Stack Overflow
**URL:** https://stackoverflow.com/questions/tagged/unreal-engine4

**Search Strategy:**
```
site:stackoverflow.com unreal engine [your question]
site:stackoverflow.com UE5 C++ [specific topic]
```

**Tags to Follow:**
- unreal-engine4
- unreal-engine5
- unrealscript
- c++

**Example Searches:**
- "UE5 UGameInstanceSubsystem example"
- "Unreal Engine component communication"
- "UPROPERTY TObjectPtr usage"

### Reddit Communities
**Subreddits:**
- r/unrealengine
- r/gamedev
- r/cpp_questions (for C++ specific)

**Search Strategy:**
```
site:reddit.com/r/unrealengine [your topic]
```

**Example Searches:**
- "Quest system architecture UE5"
- "Component vs Actor Unreal"
- "Best practices Subsystem"

---

## 4. GitHub and Source Code Examples

### GitHub Code Search
**URL:** https://github.com/search

**Search Strategies:**

1. **Search Unreal Engine Source:**
```
repo:EpicGames/UnrealEngine [class or function name]
language:C++ [your topic]
```

2. **Search Community Projects:**
```
language:C++ unreal engine quest system
language:C++ UE5 subsystem example
language:C++ unreal component pattern
```

3. **Find Similar Implementations:**
```
"UGameInstanceSubsystem" language:C++
"DECLARE_DYNAMIC_MULTICAST_DELEGATE" language:C++
"UActorComponent" "BeginPlay" language:C++
```

### Useful GitHub Repositories

**Epic Games Official:**
- https://github.com/EpicGames/UnrealEngine (requires access)
- https://github.com/EpicGames (public repos)

**Community Projects:**
- Search for: "unreal engine quest system"
- Search for: "UE5 gameplay framework"
- Search for: "unreal engine C++ examples"

**Sample Projects:**
- ActionRPG (Epic's sample)
- Lyra Starter Game (UE5 sample)
- Community quest systems

---

## 5. Video Tutorials and Courses

### YouTube Channels

**Official:**
- Unreal Engine (Official Channel)
- Epic Games Dev Community

**Community Educators:**
- CodeLikeMe
- Ryan Laley
- Matt Aspland
- Gorka Games
- UNF Games

**Search Strategy:**
```
"Unreal Engine 5 C++" [your topic]
"UE5 tutorial" [specific system]
"Unreal Engine subsystem tutorial"
```

### Udemy / Coursera
- Search for UE5 C++ courses
- Look for specific system tutorials
- Check course ratings and recency

---

## 6. Technical Blogs and Articles

### Dev.to and Medium
**Search:**
```
site:dev.to unreal engine [topic]
site:medium.com unreal engine C++ [topic]
```

### Unreal Engine Community Wiki
**URL:** https://unrealcommunity.wiki/

- Community-maintained documentation
- Tutorials and guides
- Code snippets

### Tom Looman's Blog
**URL:** https://www.tomlooman.com/

- Excellent UE C++ tutorials
- Best practices
- Advanced topics

---

## 7. Discord and Real-Time Communities

### Unreal Slackers Discord
- Large Unreal Engine community
- C++ help channels
- Quick answers from experienced devs

### Official Unreal Engine Discord
- Official support
- Community channels
- Event announcements

---

## 8. Research Documentation Template

### Create Research Notes
```markdown
# Research: [Topic]

**Date:** YYYY-MM-DD
**Researcher:** [Name]
**Context:** [Why this research was needed]

---

## Problem Statement
[Describe the problem or question]

## Research Sources

### Official Documentation
- [Link 1] - [Summary of findings]
- [Link 2] - [Summary of findings]

### Community Solutions
- [Stack Overflow link] - [Solution summary]
- [Forum thread] - [Approach discussed]

### Code Examples
- [GitHub repo] - [Implementation details]
- [Sample project] - [How they solved it]

## Key Findings

### Approach 1: [Name]
**Pros:**
- [Advantage 1]
- [Advantage 2]

**Cons:**
- [Disadvantage 1]

**Implementation:**
```cpp
// Code example
```

### Approach 2: [Name]
**Pros:**
- [Advantage 1]

**Cons:**
- [Disadvantage 1]

**Implementation:**
```cpp
// Code example
```

## Recommended Solution
[Which approach to use and why]

## Implementation Plan
1. [Step 1]
2. [Step 2]
3. [Step 3]

## References
- [All links used]

---

**Status:** [Resolved/Ongoing/Blocked]
**Next Steps:** [What to do next]
```

---

## 9. Research Checklist

### Before Starting Research
- [ ] Clearly define the problem or question
- [ ] Identify which Unreal Engine version (5.7)
- [ ] Note which system is involved (Quest, AI, etc.)
- [ ] List what has already been tried
- [ ] Gather any error messages or logs

### During Research
- [ ] Start with official Unreal Engine documentation
- [ ] Check Epic Games forums for similar issues
- [ ] Search Stack Overflow for solutions
- [ ] Look for GitHub code examples
- [ ] Check Reddit for community discussions
- [ ] Watch relevant YouTube tutorials if needed
- [ ] Document all findings in research notes

### After Research
- [ ] Summarize key findings
- [ ] Compare different approaches
- [ ] Select recommended solution
- [ ] Create implementation plan
- [ ] Document all references
- [ ] Share findings with team (if applicable)

---

## 10. Common Research Queries for cplus Project

### Quest System Research
```
Topics to research:
- "UE5 quest system architecture"
- "Unreal Engine subsystem best practices"
- "Component-based quest system"
- "GameplayTags for quest categorization"
- "Quest progress tracking UE5"
- "Delegate-based event system Unreal"
```

### AI System Research
```
Topics to research:
- "UE5 AI controller C++"
- "State Tree vs Behavior Tree UE5"
- "AI perception system Unreal"
- "NPC navigation best practices"
- "AI spawning and pooling"
```

### Performance Research
```
Topics to research:
- "Unreal Engine performance optimization C++"
- "Tick vs event-driven Unreal"
- "Component performance best practices"
- "Memory management UE5"
- "Profiling Unreal Engine C++"
```

---

## 11. Research Tools and Techniques

### Web Search Operators
```
# Exact phrase
"UGameInstanceSubsystem"

# Site-specific search
site:docs.unrealengine.com subsystem

# Exclude results
unreal engine quest -blueprint

# File type
filetype:pdf unreal engine C++ guide

# Time range
unreal engine 5.7 after:2024
```

### Browser Extensions
- Unreal Engine Documentation Search
- GitHub Code Search
- Stack Overflow Quick Search

### Documentation Tools
- Dash (macOS) / Zeal (Windows) - Offline documentation
- DevDocs.io - Multiple documentation sources

---

## 12. Research Best Practices

### DO ✅
- Start with official documentation
- Verify information from multiple sources
- Check date of information (prefer recent for UE5)
- Test solutions in isolated environment first
- Document your findings
- Share useful discoveries with team
- Bookmark useful resources

### DON'T ❌
- Trust single source without verification
- Use outdated UE4 solutions without testing
- Copy code without understanding it
- Ignore official documentation
- Skip testing before implementing
- Forget to document your research

---

## 13. Quick Reference

### Essential URLs
```
Official Docs: https://docs.unrealengine.com/
API Reference: https://docs.unrealengine.com/5.7/en-US/API/
Forums: https://forums.unrealengine.com/
Learning: https://dev.epicgames.com/community/learning
GitHub: https://github.com/EpicGames
Community Wiki: https://unrealcommunity.wiki/
```

### Search Templates
```powershell
# Google search
"Unreal Engine 5.7 C++ [your topic]"

# Stack Overflow
site:stackoverflow.com UE5 [your question]

# GitHub
language:C++ "Unreal Engine" [your topic]

# Reddit
site:reddit.com/r/unrealengine [your topic]

# YouTube
"UE5 C++ tutorial" [your topic]
```

---

## 14. Research Output

### Save Research Results
```powershell
# Create research document
$today = Get-Date -Format 'yyyy-MM-dd'
$topic = "[YourTopic]"
New-Item "docs/research/$today-$topic.md" -ItemType File -Force
```

### Share Findings
- Update project documentation
- Add to CHANGELOG if implementing solution
- Create memory for important discoveries
- Share in team chat/documentation

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
