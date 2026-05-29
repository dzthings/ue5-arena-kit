# ArenaKit — Quick Start

Get from clone to playable in under 5 minutes.

---

## Prerequisites

- Unreal Engine 5.7 installed via the Epic Games Launcher
- JetBrains Rider for Unreal **or** Visual Studio 2022 with the Unreal Engine workload
- Git with Git LFS (`git lfs install` if not already set up)

---

## 1. Create a new project from the template

On GitHub, click **"Use this template"** → **"Create a new repository"**.  
Name it whatever your project is called. Clone it locally.

```
git clone https://github.com/<you>/<your-project>.git
cd <your-project>
```

Blueprint wrappers, input assets, and the arena level are all committed — no manual Content setup required.

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

## 4. Hit Play

- WASD moves the character
- Mouse rotates the character to face the cursor
- Left click fires a projectile
- Enemies chase the player; contact deals damage; projectile hit kills them

---

## What's next

See [WhatIsIncluded.md](WhatIsIncluded.md) for the full class reference and extension points.  
Back to [README](../README.md).
