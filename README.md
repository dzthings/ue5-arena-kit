# ArenaKit — UE5 Arena Starter Template

A minimal Unreal Engine 5.7 C++ project for top-down arena games. Clone it, hit Play in minutes, build your game on top.

Includes a player character with WASD movement and cursor-facing aim, a physics projectile, a chasing AI enemy with contact damage, and a NavMesh-ready level. No Starter Content, no Manny, no baggage.

---

## Getting started

See **[docs/QuickStart.md](docs/QuickStart.md)** for the full walkthrough (clone → rename → open → play).

Short version:

1. Click **"Use this template"** on GitHub → create your repo → clone it
2. *(Optional)* `.\Tools\Rename-Module.ps1 -NewName YourGame` to rename the module
3. Open `ArenaKit.uproject` in UE5 → click **Yes** to rebuild
4. Create the Blueprint wrappers and input assets in the editor (one-time, ~5 min — see QuickStart)
5. Hit Play

---

## What's included

| System | C++ Class | Details |
|--------|-----------|---------|
| Player character | `AKitCharacter` | WASD + left stick, cursor + right stick aim, fire rate, death→restart |
| Projectile | `AKitProjectile` | Physics-driven, damage on hit, 3 s lifespan |
| AI enemy | `AKitEnemy` | Chases player via NavMesh, contact damage with cooldown |
| AI controller | `AKitAIController` | `MoveToActor` on 0.25 s timer |
| Game mode | `AKitGameMode` | Death→restart loop, configurable delay |
| Enemy spawner | `AKitEnemySpawner` | Maintains live enemy count, NavMesh-aware spawn, auto-respawn on death |
| HUD | `WBP_KitHUD` | Health bar + numeric readout, wired to `AKitCharacter` |

Full class reference and extension points: **[docs/WhatIsIncluded.md](docs/WhatIsIncluded.md)**

---

## Project structure

```
ArenaKit.uproject
│
├── Source/
│   ├── ArenaKit.Target.cs
│   ├── ArenaKitEditor.Target.cs
│   └── ArenaKit/
│       ├── ArenaKit.Build.cs
│       ├── Public/
│       │   ├── KitCharacter.h
│       │   ├── KitEnemy.h
│       │   ├── KitProjectile.h
│       │   ├── KitAIController.h
│       │   ├── KitGameMode.h
│       │   └── KitEnemySpawner.h
│       └── Private/
│           ├── KitCharacter.cpp
│           ├── KitEnemy.cpp
│           ├── KitProjectile.cpp
│           ├── KitAIController.cpp
│           ├── KitGameMode.cpp
│           └── KitEnemySpawner.cpp
│
├── Content/                        ← created / populated in the UE Editor
│   ├── Blueprints/
│   │   ├── BP_KitCharacter         (parent: AKitCharacter)
│   │   ├── BP_KitEnemy             (parent: AKitEnemy)
│   │   ├── BP_KitProjectile        (parent: AKitProjectile)
│   │   └── BP_KitGameMode          (parent: AKitGameMode)
│   ├── UI/
│   │   └── WBP_KitHUD              (health bar + HP readout — added to viewport in AKitCharacter)
│   ├── Input/
│   │   ├── IMC_Default             (Input Mapping Context)
│   │   ├── IA_Fire                 (Digital bool — Left Mouse Button)
│   │   └── Actions/
│   │       └── IA_Move             (Axis2D Vector2D — WASD)
│   ├── Maps/
│   │   └── L_ArenaKit              (floor, walls, lighting, NavMesh, enemies)
│   └── Materials/
│       ├── M_Floor
│       ├── M_Wall
│       ├── M_Character
│       ├── M_Enemy
│       └── M_Projectile
│
├── Tools/
│   └── Rename-Module.ps1           (replaces ArenaKit → YourGame everywhere)
│
└── docs/
    ├── QuickStart.md
    └── WhatIsIncluded.md
```

> **Note:** `.uasset` and `.umap` files are binary and not tracked in git. The `Content/` folder layout above shows what the editor will create. The subfolder stubs are committed so the expected structure is visible on clone.

---

## Rename script

```powershell
.\Tools\Rename-Module.ps1 -NewName YourGame
```

Replaces `ArenaKit`, `ARENAKIT_API`, and the `AKit` class prefix throughout all `.h`, `.cpp`, `.cs`, and `.uproject` files, renames files, and renames the `Source/ArenaKit/` folder. Run it once before creating any Blueprint assets — renaming after you've made Blueprints requires updating asset redirectors in the editor too.

---

## What this isn't

- No animation (no skeletal mesh, no ABP, no Manny)
- No death/restart loop — add `AArenaManager` when your project needs it
- No score, kill counter, or collectable system — those are game design, not foundation
- No VFX, no save system
- No Starter Content, no marketplace assets

The template is intentionally thin. Everything above that line is project-specific and belongs in your repo, not here.

---

## Docs

| Document | Contents |
|----------|----------|
| [docs/QuickStart.md](docs/QuickStart.md) | Full setup walkthrough from clone to first Play |
| [docs/WhatIsIncluded.md](docs/WhatIsIncluded.md) | Class reference, component details, extension points |
| [docs/StarterTemplate.md](docs/StarterTemplate.md) | Rationale, scope decisions, verification checklist |
