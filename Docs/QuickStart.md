# ArenaKit — Quick Start

Get from clone to playable in under 10 minutes.

---

## Prerequisites

- Unreal Engine 5.7 installed via the Epic Games Launcher
- JetBrains Rider for Unreal **or** Visual Studio 2022 with the Unreal Engine workload
- Git

---

## 1. Create a new project from the template

On GitHub, click **"Use this template"** → **"Create a new repository"**.  
Name it whatever your project is called. Clone it locally.

```
git clone https://github.com/<you>/<your-project>.git
cd <your-project>
```

---

## 2. (Optional) Rename the module

If you want your own module name instead of `ArenaKit`:

```powershell
.\Tools\Rename-Module.ps1 -NewName YourGame
```

This rewrites all source files, config, and folder names in one pass.  
Skip this step if `ArenaKit` is fine as the module name — you can always rename later.

---

## 3. Open in Unreal Engine

Double-click `ArenaKit.uproject` (or `YourGame.uproject` after rename).  
When UE asks to rebuild modules, click **Yes**.

---

## 4. Create the Content layer (one-time setup)

The C++ is complete; the Blueprint wrappers and input assets need to be created in the editor. Do this once per project clone — it takes about 5 minutes.

### Input assets

| Asset | Type | Path |
|-------|------|------|
| `IA_Move` | Input Action — Axis2D (Vector2D) | `Content/Input/Actions/IA_Move` |
| `IA_Fire` | Input Action — Digital (bool) | `Content/Input/IA_Fire` |
| `IMC_Default` | Input Mapping Context | `Content/Input/IMC_Default` |

`IMC_Default` mappings: `IA_Move` → W (Forward), S (Forward + Negate), A (Right + Swizzle + Negate), D (Right + Swizzle). `IA_Fire` → Left Mouse Button.

### Blueprints

| Blueprint | Parent C++ Class | Notes |
|-----------|-----------------|-------|
| `BP_KitCharacter` | `AKitCharacter` | Assign `/Engine/BasicShapes/Cube` to mesh; set `ProjectileClass = BP_KitProjectile` |
| `BP_KitProjectile` | `AKitProjectile` | Assign `/Engine/BasicShapes/Sphere` to mesh |
| `BP_KitEnemy` | `AKitEnemy` | Assign any mesh to BodyMesh |
| `BP_KitGameMode` | `AKitGameMode` | Set Default Pawn Class = `BP_KitCharacter` |

In **Project Settings → Maps & Modes**, set Default GameMode to `BP_KitGameMode`.

### Level

Create `Content/Maps/L_ArenaKit`. Set it as the default map in Project Settings.

Place:
- **PlayerStart** — center of the floor
- **NavMeshBoundsVolume** — covers the playable area; press **Build → Build Paths** to bake
- One or two `BP_KitEnemy` actors (Z = 80 so they sit on the floor)
- A large floor: **Add Actor → Cube**, scale to (20, 20, 0.5)

---

## 5. Hit Play

- WASD moves the character
- Mouse rotates the character to face the cursor
- Left click fires a projectile
- Enemies chase the player; contact deals damage; projectile hit kills them

---

## What's next

See [WhatIsIncluded.md](WhatIsIncluded.md) for the full class reference and extension points.
