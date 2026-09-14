# Getting Started with Cube World Modding

Welcome to the **CubeForge Modding Guide**! This guide walks you through setting up your C++ development environment from scratch, configuring your toolchain, compiling your first mod using **`cubeforge.mod-template`**, and running it inside Cube World.

---

## 🧭 Table of Contents

1. [Prerequisites](#1-prerequisites)
2. [Setting Up Your Development Environment](#2-setting-up-your-development-environment)
3. [Installing CubeForge Loader](#3-installing-cubeforge-loader)
4. [Cloning & Setting Up the Mod Project](#4-cloning--setting-up-the-mod-project)
5. [Building the Mod](#5-building-the-mod)
   - [Method A: 1-Click PowerShell Build (Recommended)](#method-a-1-click-powershell-build-recommended)
   - [Method B: Visual Studio 2022](#method-b-visual-studio-2022)
   - [Method C: Visual Studio Code](#method-c-visual-studio-code)
6. [Deploying & Testing In-Game](#6-deploying--testing-in-game)
7. [Next Steps](#7-next-steps)

---

## 1. Prerequisites

Before developing native mods for Cube World, ensure you have the following installed:

- **Operating System**: Windows 10 or 11 (64-bit).
- **Game Version**: [Cube World](https://store.steampowered.com/app/1128000/Cube_World/) (Steam Release `1.0.0-1`, 64-bit).
- **C++ Toolchain**: Visual Studio 2022 (or 2019) with C++ Desktop tools.
- **Git**: [Git for Windows](https://git-scm.com/download/win).
- **CMake**: Version 3.25 or higher (included automatically with Visual Studio).

---

## 2. Setting Up Your Development Environment

### Step 1: Install Visual Studio 2022 Community

1. Download the [Visual Studio Community Installer](https://visualstudio.microsoft.com/vs/).
2. Run the installer and select the **Desktop development with C++** workload.
3. On the right-hand **Installation details** pane, make sure the following optional components are checked:
   - ✅ **MSVC v143 - VS 2022 C++ x64/x86 build tools**
   - ✅ **Windows 10/11 SDK**
   - ✅ **C++ CMake tools for Windows**
4. Complete the installation and restart your PC if prompted.

> [!IMPORTANT]
> **x64 Architecture Required**: Cube World is a 64-bit executable. You must compile your mods as 64-bit (`x64`) binaries. 32-bit (`x86`) DLLs will fail to load.

---

## 3. Installing CubeForge Loader

The **[CubeForge Loader](https://github.com/Gildofj/cubeforge.loader)** is the native mod loader for Cube World.

1. Navigate to your Cube World installation folder (typically located at):
   ```text
   C:\Program Files (x86)\Steam\steamapps\common\Cube World\
   ```
2. Download the latest release of `CubeForgeLoader.fip` (or `CubeModLoader.fip`) from the [Releases page](https://github.com/Gildofj/cubeforge.loader/releases).
3. Place `CubeModLoader.fip` directly in the root Cube World directory alongside `Cube.exe`.
4. Create a folder named `Mods` inside your Cube World directory:
   ```text
   Cube World/
   ├── Cube.exe
   ├── CubeModLoader.fip
   └── Mods/
   ```

---

## 4. Cloning & Setting Up the Mod Project

Use GitHub's **"Use this template"** feature to create a new repository under your account, or clone it locally using Git:

```bash
git clone https://github.com/Gildofj/cubeforge.mod-template.git MyFirstMod
cd MyFirstMod
```

> [!TIP]
> **Automatic SDK Fetching**: The template uses CMake's `FetchContent` system. You do **not** need to manually clone or compile `cubeforge.sdk` — CMake will resolve and link the latest SDK headers and libraries during your first build!

---

## 5. Building the Mod

### Method A: 1-Click PowerShell Build (Recommended)

Open a PowerShell terminal in your project directory:

```powershell
# 1. Compile the Release DLL
.\build.ps1

# 2. (Optional) Compile and auto-copy directly to your Cube World Mods directory:
.\build.ps1 -InstallPath "C:\Program Files (x86)\Steam\steamapps\common\Cube World"
```

The compiled mod DLL and debug symbols (`.pdb`) will be located in:
```text
dist/Mods/CubeForgeSampleMod.dll
dist/Mods/CubeForgeSampleMod.pdb
```

---

### Method B: Visual Studio 2022

1. Launch Visual Studio 2022.
2. Click **Open a local folder** and select your `MyFirstMod` directory.
3. Visual Studio will automatically detect `CMakePresets.json` and `CMakeLists.txt`.
4. In the top toolbar target dropdown, select **`windows-release`** (or `x64-Release`).
5. Build the mod: press **`Ctrl + Shift + B`** (or select **Build -> Build All**).

> [!WARNING]
> **Never build in standard Debug mode (`_DEBUG`) without ABI compatibility flags**:
> Cube World was compiled in Release mode. Building with standard MSVC Debug mode alters STL iterator structures and heap layouts, resulting in immediate memory crashes when passing STL strings or vectors to the game.
> The template's `CMakeLists.txt` automatically sets `_ITERATOR_DEBUG_LEVEL=0` to keep Debug builds ABI-compatible with the game.

---

### Method C: Visual Studio Code

1. Open the project folder in VS Code.
2. Install the following extensions:
   - **C/C++** (Microsoft)
   - **CMake Tools** (Microsoft)
3. Press `Ctrl + Shift + P` -> **CMake: Select Configure Preset** -> Select `windows-release`.
4. Press `F7` (or click **Build** on the bottom status bar).

---

## 6. Deploying & Testing In-Game

1. Copy `dist/Mods/CubeForgeSampleMod.dll` into your game's `Mods/` folder:
   ```text
   C:\Program Files (x86)\Steam\steamapps\common\Cube World\Mods\CubeForgeSampleMod.dll
   ```
2. Launch Cube World via Steam.
3. Join or start a single-player world.
4. Test the built-in commands:
   - Press `Enter` to open chat, type **`/ping`**, and press `Enter`. The mod will respond in green text: `[CubeForge] Pong! Mod template is running smoothly.`
   - Type **`/help`** to view all registered commands.
   - Type **`/boost`** to toggle +25% Attack Power scaling.
   - Type **`/heal 250`** to restore 250 Health Points.
   - Press the **`F5`** key to test hotkey detection.

---

## 7. Next Steps

Now that your development environment is set up and your first mod is running, you're ready to create custom mechanics!

👉 **Continue to the [Modding Guide & Cookbook](MODDING-GUIDE.md)** for detailed recipes on handling events, creating custom UI, spawning items, and modifying creature stats.
