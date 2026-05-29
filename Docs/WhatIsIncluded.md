# ArenaKit — What Is Included

Reference for every C++ class and system in the template.

---

## C++ Classes

### `AKitCharacter` — [Source/ArenaKit/Public/KitCharacter.h](../Source/ArenaKit/Public/KitCharacter.h)

**Base:** `ACharacter`  
**Blueprint wrapper:** `BP_KitCharacter`

Top-down player character with fixed overhead camera and cursor-facing rotation.

| Feature | Detail |
|---------|--------|
| Camera | Spring arm (1200 units, -60° pitch, absolute rotation so it never spins) |
| Movement | WASD via Enhanced Input; world-space axes, not character-relative |
| Aiming | Cursor ray intersected with horizontal plane at character's feet — character yaw snaps to face hit point |
| Firing | Spawns `ProjectileClass` at muzzle socket (falls back to forward offset `MuzzleOffset`) |
| Health | `MaxHealth` (default 100); logs damage to Output Log; logs death |

**Extension points:**
- Swap `ProjectileClass` in `BP_KitCharacter` to change what fires
- Add a skeletal mesh and a "muzzle" socket for precise spawn position
- Override `TakeDamage` to add death logic (ragdoll, restart, etc.)

---

### `AKitEnemy` — [Source/ArenaKit/Public/KitEnemy.h](../Source/ArenaKit/Public/KitEnemy.h)

**Base:** `APawn`  
**Blueprint wrapper:** `BP_KitEnemy`

AI pawn that chases the player and deals contact damage on overlap.

| Feature | Detail |
|---------|--------|
| Collision | `UCapsuleComponent` root (40r × 80hh), `BlockAll` profile |
| Mesh | `UStaticMeshComponent` — visual only, `NoCollision` |
| Contact damage | `USphereComponent` (55r) overlap-only; fires `OnDamageSphereOverlap`; 0.75 s cooldown per hit |
| Movement | `UFloatingPawnMovement`, max speed 300 |
| AI | Auto-possesses `AKitAIController`; works for both placed and spawned instances |
| Health | `MaxHealth` (default 100); destroys self at 0 |

**Extension points:**
- Increase `ContactDamage` or `MaxHealth` in the Blueprint Details panel
- Scale `BodyMesh` (not the actor) to change visual size without breaking capsule/nav
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
| Lifespan | 3 seconds; destroys on any hit |

**Extension points:**
- Adjust `Damage`, `InitialSpeed`, `InitialLifeSpan` in `BP_KitProjectile`
- Add a Niagara trail component for game feel

---

### `AKitAIController` — [Source/ArenaKit/Public/KitAIController.h](../Source/ArenaKit/Public/KitAIController.h)

**Base:** `AAIController`

Polls `MoveToActor` toward the player pawn every 0.25 s using the NavMesh.

**Extension points:**
- Reduce poll interval for more reactive tracking
- Add detection range check before issuing move so enemies only chase when nearby

---

### `AKitGameMode` — [Source/ArenaKit/Public/KitGameMode.h](../Source/ArenaKit/Public/KitGameMode.h)

**Base:** `AGameModeBase`

Empty shell. Set Default Pawn Class in `BP_KitGameMode`.

**Extension points:**
- Add `AArenaManager` (death counter, restart logic, wave spawning)

---

## Input Setup

Input assets are loaded in `AKitCharacter`'s constructor via `ConstructorHelpers::FObjectFinder`. The C++ expects these exact Content paths:

| Asset | Content Path |
|-------|-------------|
| `IMC_Default` | `/Game/Input/IMC_Default` |
| `IA_Move` | `/Game/Input/Actions/IA_Move` |
| `IA_Fire` | `/Game/Input/IA_Fire` |

If any path is wrong, movement or fire silently fails. Check the Output Log on Play for load errors.

---

## Rename Script

`Tools/Rename-Module.ps1 -NewName YourGame` — replaces `ArenaKit`, `ARENAKIT_API`, and `AKit` class prefixes across all `.h`, `.cpp`, `.cs`, and `.uproject` files, then renames files and the `Source/ArenaKit/` folder. Run it once before creating any Blueprints.

---

## What Is Deliberately Excluded

| Excluded | Why |
|----------|-----|
| Skeletal mesh / Manny | Animation is a project milestone, not template scope |
| Animation Blueprint | Same reason |
| Starter Content | It previously overwrote input assets; never again |
| Any marketplace / Fab content | Add per-project, never to the template |
| Death/restart loop | Lives in `AArenaManager`; add it when you need it |
| Damage numbers, VFX | Game feel pass, not foundation |
