# StarterTemplate — Rationale and Scope

Documents why this template exists, what it includes, and what it deliberately excludes. Read this before modifying the template.

---

## Why a separate template repo

ue-playground accumulates game-specific systems, StarterContent, Manny mesh, in-progress experiments, and history from all of that. Cloning it as a starting point for a new project means inheriting all that noise.

This template is the extracted core — only the systems that every arena project needs. New projects clone it via GitHub's "Use this template" and start with a clean commit history and a clean Content folder.

---

## What was ported

All production C++ classes from ue-playground, renamed to the `AKit` prefix:

| Template Class | Ported From | Notes |
|---------------|-------------|-------|
| `AKitCharacter` | `AArenaCharacter` | Rename only |
| `AKitEnemy` | `APlaygroundEnemy` | Rename; `APlaygroundPawn` cast → `AKitCharacter` cast |
| `AKitProjectile` | `APlaygroundProjectile` | Rename only |
| `AKitAIController` | `APlaygroundAIController` | Rename only |
| `AKitGameMode` | `APlaygroundGameMode` | Rename; removed startup debug log |

`APlaygroundPawn` was not ported — it's legacy, superseded by `AArenaCharacter`/`AKitCharacter`.

---

## What was not ported and why

| Excluded | Reason |
|----------|--------|
| `APlaygroundPawn` | Legacy class, superseded by `AArenaCharacter` |
| StarterContent | Content pack that previously overwrote input assets |
| Manny mesh / skeleton | Animation is a milestone, not a template concern |
| Third Person content | Same — adds noise, no value in a clean template |
| RiderLink plugin binaries | Too large for git; user installs it once from the editor |
| Game-specific design docs | Those live in ue-playground |

---

## Module name rationale

`ArenaKit` was chosen because:
- Short, no collisions with Epic module names
- Clearly non-game-specific (doesn't imply any genre beyond "arena")
- The `AKit` class prefix visually distinguishes template base classes from project subclasses
- The rename script replaces it cleanly in one pass

---

## Input asset paths

`AKitCharacter` loads input assets at these exact paths via `ConstructorHelpers`:

```
/Game/Input/IMC_Default
/Game/Input/Actions/IA_Move
/Game/Input/IA_Fire
```

These paths are intentionally identical to ue-playground's paths so migration muscle memory transfers. If you rename these assets in a downstream project, update the paths in `KitCharacter.cpp`.

---

## Enemy contact damage — cast decision

`KitEnemy.cpp` casts the overlapping actor to `AKitCharacter` before applying contact damage. This is intentional: enemies should only damage the player, not each other or stray actors. The cast is the type-check. In a downstream project, if the player class is subclassed from `AKitCharacter`, the cast still works.

---

## HUD widget — scope decision

`WBP_KitHUD` (a UMG widget) is included to demonstrate the C++ → UMG data binding pattern. It shows player health only. No kill counter, no score, no timer — those are game design decisions that belong in the downstream project, not the template.

The binding reads `CurrentHealth` and `MaxHealth` directly from `AKitCharacter`. Override `TakeDamage` in a subclass if you need to broadcast health changes as a delegate instead.

---

## GitHub Template setup

The repo is marked as a GitHub Template Repository (Settings → check "Template repository"). Use "Use this template" instead of forking — this gives a fresh repo with no fork relationship and a clean first commit.

Do not push commits to `ue5-arena-kit` that contain project-specific code. The template should stay generic.

---

## Verification before tagging a template update

Before tagging a new version of the template:
- [ ] Opens in UE 5.7 without errors
- [ ] WASD moves, mouse aims, left click fires
- [ ] Enemy chases, takes projectile damage, dies
- [ ] Player takes contact damage from enemy
- [ ] NavMesh bakes (green overlay with P key)
- [ ] HUD displays player health; health bar decreases on contact damage
- [ ] No compile errors or output log warnings on Play
- [ ] Rename script: run with a test name, reopen — all references updated, no leftover `ArenaKit` strings in Source/
