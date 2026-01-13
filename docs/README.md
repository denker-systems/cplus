# cplus - Unreal Engine 5.7 Project

A multi-variant game project built with Unreal Engine 5.7, featuring quest systems, shooter mechanics, and horror elements.

---

## Project Overview

**cplus** is a flexible game framework that supports multiple gameplay variants:
- **Shooter Variant**: Fast-paced combat with weapons and enemies
- **Horror Variant**: Atmospheric horror with psychological elements
- **Quest System**: Central quest management system used across variants

### Technology Stack
- **Engine**: Unreal Engine 5.7
- **Language**: C++
- **Build System**: Unreal Build Tool (UBT)
- **Version Control**: Git
- **Documentation**: Markdown with Windsurf workflows

---

## Quick Start

### Prerequisites
- Unreal Engine 5.7 installed at `C:\ue5.7\UE_5.7`
- Visual Studio 2022 with C++ development tools
- Git for version control

### Setup
1. Clone the repository
2. Open `cplus.uproject` in Unreal Engine
3. Build the project (Development configuration)
4. Review documentation in `docs/` folder

### Build Commands
```powershell
# Development build
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"

# Clean build
Remove-Item -Recurse -Force "Intermediate","Binaries" -ErrorAction SilentlyContinue
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

---

## Project Structure

```
cplus/
├── Source/
│   └── cplus/
│       ├── cplus.Build.cs          # Module dependencies
│       ├── cplus.h/cpp             # Module definition
│       ├── QuestSystem/            # Quest management system
│       │   ├── Core/               # Core quest classes
│       │   ├── Components/         # Quest components
│       │   ├── Tasks/              # Quest task types
│       │   └── Interfaces/         # Quest interfaces
│       ├── Variant_Shooter/        # Shooter game variant
│       └── Variant_Horror/         # Horror game variant
├── Content/                        # Unreal assets
│   ├── Blueprints/                 # Blueprint assets
│   ├── Maps/                       # Level files
│   ├── Materials/                  # Material assets
│   └── Data/                       # Data assets
├── Config/                         # Configuration files
├── docs/                           # Documentation
│   ├── dev/                        # Development docs
│   │   ├── sessions/               # Daily session reports
│   │   └── DEVLOG_YYYY-MM.md       # Monthly devlogs
│   ├── CHANGELOG.md                # Project changelog
│   └── ROADMAP.md                  # Project roadmap
├── .windsurf/                      # Windsurf rules and workflows
└── cplus.uproject                  # Project file
```

---

## Core Systems

### Quest System
The quest system provides:
- **Quest Definition**: Data-driven quest configuration
- **Quest Tracking**: Real-time quest progress monitoring
- **Quest Givers**: NPCs that offer quests
- **Quest Targets**: Objects/NPCs for quest objectives
- **Quest Events**: Delegate-based event system

### Component Architecture
- **UQuestTrackerComponent**: Track player's active quests
- **UQuestGiverComponent**: Offer quests to players
- **UQuestTargetComponent**: Notify quest system on interaction

### Interfaces
- **IQuestInteractable**: Objects that can be interacted with for quests
- **IQuestKillable**: Objects that can be killed for quest objectives

---

## Development Guidelines

### Code Standards
- All code follows Unreal Engine coding standards
- All documentation and comments in English
- Use UPROPERTY for all UObject references
- Use TObjectPtr for smart pointer behavior
- Avoid std:: containers, use Unreal containers

### Naming Conventions
- Classes: `A` prefix for Actors, `U` for Objects, `I` for Interfaces
- Functions: PascalCase
- Variables: PascalCase for UPROPERTY, camelCase for locals
- Files: Match class name exactly

### Git Workflow
- Feature branches: `feature/[feature-name]`
- Conventional commits: `feat(scope): description`
- Documentation updated with every change

---

## Documentation

### Development Documentation
- **Session Reports**: Daily progress in `docs/dev/sessions/`
- **DEVLOG**: Monthly summaries in `docs/dev/`
- **CHANGELOG**: All changes tracked in `docs/CHANGELOG.md`
- **ROADMAP**: Project plan in `docs/ROADMAP.md`

### API Documentation
- Code documentation follows Doxygen style
- All public functions documented
- Complex algorithms explained

### Windsurf Workflows
Custom workflows for:
- Session initialization (`/init-session`)
- Building (`/build`)
- Debugging (`/debug`)
- New features (`/new-feature`)
- Asset management (`/add-asset`)
- Git commits (`/git-commit`)
- Documentation updates (`/update-docs`)

---

## Testing

### Build Verification
```powershell
# Verify build works
& "C:\ue5.7\UE_5.7\Engine\Build\BatchFiles\Build.bat" cplusEditor Win64 Development "C:\Users\Calle\Documents\Unreal Projects\cplus\cplus.uproject"
```

### Test Coverage
- Unit tests for core quest logic
- Integration tests for component interaction
- Performance tests for quest tracking
- User acceptance tests

---

## Performance Considerations

### Optimization
- Use event-driven architecture instead of polling
- Cache cast results
- Use appropriate container types
- Profile with Unreal Insights

### Memory Management
- Use TObjectPtr for UPROPERTY references
- Avoid raw pointers to UObjects
- Use Unreal containers (TArray, TMap, TSet)

---

## Contributing

1. Create feature branch
2. Implement changes with tests
3. Update documentation
4. Submit pull request
5. Code review and merge

---

## License

[Add your license here]

---

## Contact

[Add contact information]

---

**Version:** 0.2.0 (In Development)  
**Last Updated:** 2026-01-13  
**Engine:** Unreal Engine 5.7
