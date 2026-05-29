# ArenaKit — What Is Included

Reference for every C++ class and system in the template.

---

## C++ Classes

### `AKitCharacter` — [Source/ArenaKit/Public/KitCharacter.h](../Source/ArenaKit/Public/KitCharacter.h)

**Base:** `ACharacter`  
**Blueprint wrapper:** `BP_KitCharacter`

Top-down player character with fixed overhead camera, controller-first aim, and fire rate limiting.

| Feature | Detail |
|---------|--------|
| Camera | Spring arm with lag (`CameraLagSpeed = 8`); arm length and pitch exposed as `EditDefaultsOnly` — change in BP_KitCharacter Details panel, `OnConstruction` syncs to the spring arm |
| Movement | WASD / left stick via Enhanced Input; world-space axes, not character-relative |
| Aiming | Right stick takes priority (twin-stick); falls back to cursor ray-plane intersection when stick is neutral |
| Firing | Rate-limited by `FireRate` (default 0.2 s); spawns `ProjectileClass` at muzzle socket or forward offset |
| Health | `MaxHealth` (default 100); on death calls `AKitGameMode::NotifyPlayerDied()` |
| Quit | ESC / gamepad Start calls `quit` console command |

**Camera tuning** — override in `BP_KitCharacter` without touching C++:

| Property | Default | Effect |
|----------|---------|--------|
| `CameraArmLength` | 1200 | Zoom level |
| `CameraPitch` | -60° | Camera angle (-90 = directly overhead) |

**Extension points:**
- Swap `ProjectileClass` in `BP_KitCharacter` to change what fires
- Adjust `FireRate`, `MoveSpeed`, `MuzzleOffset` in the Blueprint Details panel
- Add a skeletal mesh with a `muzzle` socket for precise projectile spawn position
- Override `TakeDamage` in a subclass to add death effects before calling Super

---

### `AKitEnemy` — [Source/ArenaKit/Public/KitEnemy.h](../Source/ArenaKit/Public/KitEnemy.h)

**Base:** `APawn`  
**Blueprint wrapper:** `BP_KitEnemy`

AI pawn that chases the player and deals contact damage on overlap.

| Feature | Detail |
|---------|--------|
| Collision | `UCapsuleComponent` root (40r × 80hh), `BlockAll` profile |
| Mesh | `UStaticMeshComponent` — visual only, `NoCollision` |
| Contact damage | `USphereComponent` (55r) overlap-only; 0.75 s cooldown per hit |
| Movement | `UFloatingPawnMovement`, max speed 300 |
| AI | Auto-possesses `AKitAIController`; works for both placed and spawned instances |
| Health | `MaxHealth` (default 100); destroys self at 0 |

**Extension points:**
- Adjust `ContactDamage`, `MaxHealth` in the Blueprint Details panel
- Scale `BodyMesh` (not the actor) to change visual size without affecting capsule or nav
- Subclass `AKitAIController` for smarter behavior

---

### `AKitProjectile` — [Source/ArenaKit/Public/KitProjectile.h](../Source/ArenaKit/Public/KitProjectile.h)

**Base:** `AActor`  
**Blueprint wrapper:** `BP_KitProjectile`

Physics-driven projectile with hit-on-impact damage.

| Feature | Detail |
|---------|--------|
| Collision | `USphereComponent` (15r), `BlockAllDynamic`; binds `OnHit` |
| Mesh | `UStaticMeshComponent`, `NoCollision` |
| Movement | `UProjectileMovementComponent`, 1500 units/s, zero gravity |
| Damage | Calls `TakeDamage` on the hit actor; skips the instigator |
| Lifespan | 3 s; destroys on any hit |

**Extension points:**
- Adjust `Damage`, speed, and lifespan in `BP_KitProjectile`
- Add a Niagara trail component for game feel

---

### `AKitAIController` — [Source/ArenaKit/Public/KitAIController.h](../Source/ArenaKit/Public/KitAIController.h)

**Base:** `AAIController`

Polls `MoveToActor` toward the player pawn every 0.25 s using the NavMesh.

**Extension points:**
- Reduce poll interval for more reactive tracking
- Add detection range check so enemies only chase when nearby

---

### `AKitGameMode` — [Source/ArenaKit/Public/KitGameMode.h](../Source/ArenaKit/Public/KitGameMode.h)

**Base:** `AGameModeBase`

Handles the death-to-restart loop. `NotifyPlayerDied()` is called by `AKitCharacter` on death; the level reloads after `RestartDelay` seconds.

| Property | Default | Effect |
|----------|---------|--------|
| `RestartDelay` | 2 s | Pause before level reload — tune in `BP_KitGameMode` |

**Extension points:**
- Add wave tracking, score, or a death screen before calling `RestartLevel()`

---

### `AKitEnemySpawner` — [Source/ArenaKit/Public/KitEnemySpawner.h](../Source/ArenaKit/Public/KitEnemySpawner.h)

**Base:** `AActor`  
**No Blueprint wrapper needed** — configure in the level Details panel.

Maintains a live enemy count. Spawns `SpawnCount` enemies at BeginPlay; when one dies, spawns a replacement after `SpawnInterval` seconds at a random NavMesh point within `SpawnRadius`.

| Property | Default | Effect |
|----------|---------|--------|
| `EnemyClass` | none | Set to `BP_KitEnemy` or a subclass |
| `SpawnCount` | 3 | Enemies kept alive simultaneously |
| `SpawnInterval` | 2 s | Delay before replacement spawn |
| `SpawnRadius` | 300 | Spawn area radius around the spawner actor |

Place one or more `AKitEnemySpawner` actors in the level. No C++ changes needed to adjust difficulty.

---

## Input Setup

Input assets are loaded in `AKitCharacter`'s constructor via `ConstructorHelpers::FObjectFinder`. The C++ expects these exact Content paths:

| Asset | Content Path | Type |
|-------|-------------|------|
| `IMC_Default` | `/Game/Input/IMC_Default` | Input Mapping Context |
| `IA_Move` | `/Game/Input/Actions/IA_Move` | Axis2D (Vector2D) |
| `IA_Aim` | `/Game/Input/Actions/IA_Aim` | Axis2D (Vector2D) |
| `IA_Fire` | `/Game/Input/IA_Fire` | Digital (bool) |
| `IA_Quit` | `/Game/Input/IA_Quit` | Digital (bool) |

**`IMC_Default` mappings:**

| Action | KB+M | Gamepad |
|--------|------|---------|
| `IA_Move` | W/A/S/D | Left stick |
| `IA_Aim` | — | Right stick |
| `IA_Fire` | Left mouse button | Right trigger |
| `IA_Quit` | Escape | Start / Menu button |

If any path is wrong the binding silently fails. Check the Output Log on Play for load errors.

---

## Rename Script

`Tools/Rename-Module.ps1 -NewName YourGame` — replaces `ArenaKit`, `ARENAKIT_API`, and `AKit` class prefixes across all `.h`, `.cpp`, `.cs`, and `.uproject` files, then renames files and the `Source/ArenaKit/` folder. Run once before creating any Blueprint assets.

---

## What Is Deliberately Excluded

| Excluded | Why |
|----------|-----|
| Skeletal mesh / Manny | Animation is a project milestone, not template scope |
| Animation Blueprint | Same reason |
| Starter Content | Previously overwrote input assets; never again |
| Any marketplace / Fab content | Add per-project, never to the template |
| Score / kill counter / collectables | Game design decisions — belong in the project |
| Damage numbers, VFX | Game feel pass, not foundation |
| Floating enemy health bars | Project-specific UI styling |

---

Back to [README](../README.md) · [QuickStart](QuickStart.md)
