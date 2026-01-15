# Animation Integration Guide (GASP -> cplus)

## Scope
This guide describes a minimal-first roadmap for integrating Epic's **Game Animation Sample Project (GASP)** concepts and/or assets into **cplus**.

Focus:
- Improve third-person locomotion quality and responsiveness.
- Establish a scalable animation architecture (unarmed + weapon states).
- Minimize risk by adopting patterns first, migrating assets second.

Non-goals:
- Rebuilding cplus gameplay systems inside the sample.
- Shipping the sample project as-is.

## What GASP Provides (High-Level)
GASP is a living sample project focused on gameplay animation systems:
- Motion Matching locomotion driven by a capsule-based movement model.
- Pose Search assets (Schema + Databases) to drive Motion Matching.
- Choosers to select/limit which Pose Search DBs are available based on gameplay context.
- Example character blueprints and animation blueprints designed to be studied and adapted.

Reference:
- https://dev.epicgames.com/documentation/en-us/unreal-engine/game-animation-sample-project-in-unreal-engine

## Decision: Patterns vs Asset Migration
Choose one of these strategies. You can start with A and later do B.

### Strategy A (Recommended First): Copy the Architecture
- Treat GASP as a reference.
- Recreate the structure in cplus using your own assets and skeleton.

Pros:
- Lowest integration risk.
- No surprise dependencies.
- Fits cplus-specific state (quests, shooter/horror modes, weapon system).

Cons:
- Slower initial visual results.

### Strategy B: Migrate Selected Assets
- Migrate a small, controlled slice of GASP animation assets and blueprints.
- Retarget to your character.

Pros:
- Fastest path to high-quality locomotion.

Cons:
- High dependency surface.
- Retarget/skeleton mismatch risks.
- Requires plugin parity and careful asset path management.

## Prerequisites (Before Touching Assets)
### Engine and Plugins
- Use the same engine version for GASP and cplus (you are on UE 5.7).
- Ensure required plugins are enabled in cplus (names can vary by version):
  - Motion Matching / Pose Search
  - Chooser
  - IK Rig / IK Retargeter

### Character Readiness
- Confirm your player mesh and skeleton setup is stable.
- Ensure your character movement settings are consistent with capsule-driven locomotion.

### Input Baseline
- Keep gameplay input separate from animation decisions.
- Animation should be driven by a small set of stable state variables (velocity, acceleration, movement mode, stance, gait, weapon state).

## Roadmap

### Phase 0: Baseline + Instrumentation (1-2 sessions)
Goal: make your current animation setup observable and safe to evolve.

- Validate you have a stable unarmed AnimBP (even if basic).
- Add a minimal animation state interface between gameplay and animation (C++/BP):
  - Stance: Standing/Crouching
  - Locomotion: Idle/Moving/InAir
  - Gait: Walk/Sprint
  - Weapon: Unarmed/Pistol/Rifle (or a generic Armed)
- Ensure `BasePlayerCharacter` never sets a null AnimClass.
  - Weapon activation should fallback if a weapon AnimClass is missing.

Exit criteria:
- You can print/log the state variables and see they match gameplay.
- Switching weapon states does not break AnimBP.

### Phase 1: Adopt GASP Concepts Without Migration (2-5 sessions)
Goal: align your architecture with what GASP expects, without importing assets.

- Build an AnimBP structure that supports:
  - Base locomotion graph
  - Overlays or weapon-specific layers
  - Clean separation between locomotion and actions (montages)
- Introduce the idea of a "database selection" step:
  - Even before Motion Matching, implement a simple "which locomotion set" selection function.
  - This mirrors Chooser usage later.

Exit criteria:
- You can switch locomotion sets (unarmed vs armed) without breaking.
- You can extend the graph without adding hard coupling to gameplay code.

### Phase 2: Bring in Motion Matching (Smallest Working Slice) (3-8 sessions)
Goal: enable Motion Matching for basic locomotion only.

Minimal slice:
- Standing locomotion only (no traversal).
- One character skeleton.
- One Pose Search Schema.
- One Pose Search Database.
- One Motion Matching node + Pose History.

Implementation notes:
- Start with a single locomotion DB, then split later.
- Do not integrate traversal until locomotion is stable.

Exit criteria:
- Motion Matching drives idle/walk/run transitions smoothly.
- No major foot sliding or pose popping.

### Phase 3: Add Choosers (Context-Driven DB Selection) (2-5 sessions)
Goal: replicate GASP's core control mechanism.

- Add Chooser-driven DB selection based on context:
  - Stance (stand/crouch)
  - Gait (walk/sprint)
  - Weapon state (unarmed/armed)
  - Movement mode (grounded/in-air)

Exit criteria:
- DB selection changes are stable and predictable.
- Switching weapon/stance does not cause invalid pose results.

### Phase 4: Weapon Overlays + Action System (ongoing)
Goal: integrate shooter/horror needs.

- Define a weapon overlay pipeline:
  - Unarmed locomotion
  - Armed locomotion
  - Weapon-specific upper body overlays (aim, recoil)
- Actions:
  - Use montages for firing/reload.
  - Consider Motion Matching-driven montage entry frame selection later (advanced).

Exit criteria:
- Shooting/reload does not destabilize locomotion.
- Weapon switching cleanly changes overlay/DB selection.

### Phase 5: Retargeting New Characters (optional)
Goal: use GASP workflow for swapping characters.

- Follow Epic's retargeted character patterns.
- Require IK Rig and Retargeter per character.

Reference (MetaHuman workflow shows the kinds of wiring/tagging involved):
- https://dev.epicgames.com/documentation/en-us/unreal-engine/adding-a-metahuman-to-the-game-animation-sample-project-in-unreal-engine

## Recommended Integration Path for cplus
If you want minimal risk:
1) Strategy A for architecture.
2) Phase 2 minimal Motion Matching slice.
3) Phase 3 Choosers.
4) Phase 4 weapon overlays.

If you want fastest visuals:
1) Strategy B but migrate only locomotion slice.
2) Retarget.
3) Replace pieces incrementally.

## Practical Migration Checklist (Strategy B)
Only migrate a controlled subset.

1) Open GASP project.
2) Identify the locomotion assets you want:
   - Pose Search Schema
   - Pose Search Databases for locomotion
   - AnimBP components that host Motion Matching
   - Any required animation sequences
3) In Content Browser, use Migrate to bring assets into cplus.
4) Enable required plugins in cplus.
5) Retarget animations to your skeleton (IK Rig + IK Retargeter).
6) Create a new AnimBP in cplus that references the migrated/retargeted assets.
7) Assign AnimBP to your character mesh.

## Common Pitfalls
- Missing plugins (Pose Search / Chooser) causing assets to fail to load.
- Retargeted skeleton mismatch (bad IK chains, wrong retarget profile).
- AnimClass swapping by weapon code to a null or incompatible AnimBP.
- Mixing gameplay state and animation state directly (keep a small interface).

## Verification Checklist (Per Phase)
- No T-pose on spawn.
- AnimBP assigned and valid in all weapon/stance states.
- Locomotion responds to:
  - start/stop
  - direction changes
  - sprint toggle
  - crouch toggle
  - jump/in-air
- Performance:
  - Motion Matching DB size is reasonable.
  - Debug tools show expected DB selection.

## Next Step (Pick One)
- Option 1: Implement Phase 0 interface variables in cplus (weapon/stance/gait/mode) and drive AnimBP with them.
- Option 2: Set up a minimal Pose Search Schema + DB in cplus and get Motion Matching running for locomotion.
