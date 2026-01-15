# Quick Review Checklist

## Before Approving

### Safety
- [ ] No raw UObject* without UPROPERTY
- [ ] All UObject access has null check
- [ ] Super:: called in all overrides
- [ ] No static UObject pointers

### Standards
- [ ] Correct class prefix (A/U/F/E/I)
- [ ] GENERATED_BODY() present
- [ ] .generated.h is last include
- [ ] English comments only

### Build
- [ ] Compiles without errors
- [ ] No new warnings
- [ ] Module dependencies correct

### Documentation
- [ ] Class has doc comment
- [ ] Public API documented
- [ ] Complex logic explained

## Red Flags (Reject)

- `new` or `delete` for UObjects
- `std::vector`, `std::string`, `std::map`
- Missing UPROPERTY on UObject pointer
- Missing Super:: call
- Swedish comments
- Hardcoded file paths
- Tick with expensive operations
