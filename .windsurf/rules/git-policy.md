# Git Policy

> **Destination:** `.windsurf/rules/git-policy.md`

---
trigger: always_on
description: Git workflow rules and policies for cplus project
---

# Git Policy

> Critical rules for Git operations in Unreal Engine project

## ⚠️ CRITICAL GIT RULES - NEVER BREAK THESE

### 1. **NEVER git commit without explicit user instruction**
- AI must ALWAYS ask before committing
- User must explicitly say "commit" or "create commit"
- No automatic commits, even if changes are ready

### 2. **NEVER git push without explicit user instruction**
- AI must ALWAYS ask before pushing
- User must explicitly say "push" or "push to GitHub"
- No automatic push, even after commit

### 3. **NEVER git add without explicit user instruction**
- AI must ALWAYS ask before staging files
- User must explicitly say "add files" or "stage changes"
- Exception: Can show `git status` to inform user

### 4. **NEVER force push**
- `git push --force` is FORBIDDEN
- `git push --force-with-lease` is FORBIDDEN
- If history needs rewriting, discuss with user first

### 5. **NEVER modify Git history without permission**
- No `git rebase` without explicit instruction
- No `git reset --hard` without explicit instruction
- No `git commit --amend` without explicit instruction

---

## Git Workflow

### Branch Strategy

```
main (or master)
├── feature/[feature-name]    # New features
├── fix/[bug-name]             # Bug fixes
├── refactor/[system-name]     # Code refactoring
└── docs/[doc-name]            # Documentation updates
```

### Allowed Git Operations (Without Permission)

These operations are READ-ONLY and safe:

```bash
git status              # Check status
git log                 # View history
git diff                # View changes
git branch              # List branches
git remote -v           # View remotes
git show                # Show commit details
```

### Operations Requiring User Permission

**ALWAYS ask before:**

```bash
git add                 # Stage files
git commit              # Create commit
git push                # Push to remote
git pull                # Pull from remote
git merge               # Merge branches
git rebase              # Rebase branches
git reset               # Reset changes
git checkout            # Switch branches
git branch -d           # Delete branch
git stash               # Stash changes
```

---

## Conventional Commits

### Format

```
<type>(<scope>): <description>

[optional body]

[optional footer]
```

### Types

| Type | Usage | Example |
|------|-------|---------|
| `feat` | New feature | `feat(quest): Add reward system` |
| `fix` | Bug fix | `fix(ai): NPC navigation issue` |
| `docs` | Documentation | `docs(readme): Update setup guide` |
| `style` | Code style | `style(quest): Fix indentation` |
| `refactor` | Refactoring | `refactor(quest): Simplify logic` |
| `test` | Tests | `test(quest): Add unit tests` |
| `chore` | Maintenance | `chore(build): Update dependencies` |
| `perf` | Performance | `perf(ai): Optimize pathfinding` |

### Scopes for Unreal Engine

| Scope | Area |
|-------|------|
| `core` | Core systems |
| `quest` | Quest system |
| `shooter` | Shooter variant |
| `horror` | Horror variant |
| `ai` | AI system |
| `ui` | User interface |
| `build` | Build system |
| `docs` | Documentation |

---

## Pre-Commit Checklist

Before asking to commit, verify:

- [ ] Code compiles (Development build)
- [ ] No build artifacts staged (Binaries/, Intermediate/)
- [ ] Commit message follows conventional format
- [ ] Related documentation updated
- [ ] No sensitive data (API keys, passwords)
- [ ] .gitignore is correct

---

## Git LFS

### Tracked File Types

Large files (>100MB) automatically tracked by Git LFS:

- `.uasset`, `.umap` (Unreal assets)
- `.fbx`, `.obj`, `.blend` (3D models)
- `.png`, `.jpg`, `.tga`, `.psd` (Textures)
- `.wav`, `.mp3`, `.ogg` (Audio)
- `.mp4`, `.mov`, `.avi` (Video)
- `.dll`, `.exe`, `.so` (Binaries)

### LFS Commands

```bash
# Check LFS status
git lfs ls-files

# Track new file type
git lfs track "*.extension"

# Verify LFS is working
git lfs status
```

---

## File Management

### ✅ DO Commit

- Source code (`.h`, `.cpp`)
- Build scripts (`.Build.cs`, `.Target.cs`)
- Project file (`.uproject`)
- Configuration (`.ini` files in Config/)
- Content assets (`.uasset`, `.umap`)
- Documentation (`.md` files)
- Git configuration (`.gitignore`, `.gitattributes`)

### ❌ DO NOT Commit

- Build artifacts (`Binaries/`, `Intermediate/`)
- Editor cache (`DerivedDataCache/`, `Saved/`)
- IDE files (`.vs/`, `.vscode/`, `.idea/`)
- Temporary files (`*.tmp`, `*.bak`)
- User settings (`*.sln`, `*.suo`)
- Logs (`*.log`)

---

## GitHub Desktop Integration

### Workflow

1. **Make changes** in code
2. **Review changes** in GitHub Desktop
3. **Ask AI:** "Should I commit these changes?"
4. **AI reviews** and suggests commit message
5. **User decides** to commit or not
6. **User commits** via GitHub Desktop
7. **User pushes** when ready

### Benefits

- Visual diff viewer
- Easy branch management
- Automatic authentication
- Conflict resolution UI
- Git LFS handled automatically

---

## Emergency Procedures

### If Accidentally Committed

```bash
# Undo last commit (keep changes)
git reset --soft HEAD~1

# Undo last commit (discard changes) - DANGEROUS
git reset --hard HEAD~1
```

### If Accidentally Pushed

```bash
# Revert commit (creates new commit)
git revert HEAD

# Then push the revert
git push
```

### If Merge Conflict

1. **DO NOT panic**
2. **Ask user** how to resolve
3. **Use GitHub Desktop** conflict resolution
4. **Test build** after resolution
5. **Commit** resolution with user permission

---

## Documentation Sync

### After Every Commit

Update these files:

- `docs/CHANGELOG.md` - Add changes under [Unreleased]
- `docs/dev/sessions/YYYY-MM-DD.md` - Log commit in session report
- `docs/dev/DEVLOG_YYYY-MM.md` - Update monthly log

### Commit Message Template

```bash
feat(quest): Add quest reward system

- Implement reward calculation
- Add reward distribution to QuestSubsystem
- Create reward UI notifications
- Update quest completion flow

Closes #123
```

---

## Best Practices

### DO ✅

- Commit related changes together
- Write descriptive commit messages
- Update documentation with code
- Review changes before committing
- Use branches for features
- Keep commits atomic and focused

### DON'T ❌

- Commit work-in-progress without marking as WIP
- Mix unrelated changes in one commit
- Commit commented-out code
- Commit debug prints
- Commit large binary files without LFS
- Commit without testing

---

## AI Assistant Rules Summary

As an AI assistant, I must:

1. ✅ **CAN** show git status and diffs
2. ✅ **CAN** suggest commit messages
3. ✅ **CAN** review changes before commit
4. ❌ **CANNOT** run `git add` without permission
5. ❌ **CANNOT** run `git commit` without permission
6. ❌ **CANNOT** run `git push` without permission
7. ❌ **CANNOT** modify Git history without permission
8. ✅ **MUST** always ask before any write operation

---

**Version:** 1.0  
**Last Updated:** 2026-01-13  
**Project:** cplus (Unreal Engine 5.7)
