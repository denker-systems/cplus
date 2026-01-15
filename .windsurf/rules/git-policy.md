---
trigger: always_on
description: Git safety rules - NEVER break these
---

# Git Policy

<critical_rules>
NEVER run without explicit user instruction:
- git add
- git commit  
- git push
- git merge
- git rebase
- git reset
- git checkout (branch switch)

NEVER force push (git push --force)
NEVER modify Git history without permission
ALWAYS ask before any write operation to Git
</critical_rules>

<safe_operations>
These are read-only and safe to run:
- git status
- git log
- git diff
- git branch
- git remote -v
- git show
</safe_operations>

<commit_format>
Type(scope): description

Types: feat, fix, docs, style, refactor, test, chore, perf
Scopes: core, quest, shooter, horror, ai, ui, build, docs

Examples:
- feat(quest): Add quest reward system
- fix(ai): NPC navigation stuck in walls
- docs(readme): Update installation instructions
</commit_format>

<pre_commit_checklist>
Before suggesting commit:
- [ ] Code compiles (Development build)
- [ ] No new warnings
- [ ] No build artifacts staged (Binaries/, Intermediate/)
- [ ] Documentation updated if needed
- [ ] Commit message follows conventional format
</pre_commit_checklist>

<gitignore>
Never commit:
- Binaries/
- Intermediate/
- Saved/
- .vs/
- *.log
- DerivedDataCache/
</gitignore>