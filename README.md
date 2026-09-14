# CubeForge Mod Template

[![C++20](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)](https://www.microsoft.com/windows)
[![Toolchain](https://img.shields.io/badge/Compiler-MSVC%20%28x64%29-brightgreen.svg)](https://visualstudio.microsoft.com/)
[![CMake](https://img.shields.io/badge/CMake-3.25%2B-064F8C.svg)](https://cmake.org/)
[![CubeForge SDK](https://img.shields.io/badge/CubeForge%20SDK-v2.0.0-purple.svg)](https://github.com/Gildofj/cubeforge.sdk)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

The official, production-ready starter template for creating native C++20 mods for **Cube World** (Steam Release `1.0.0-1`).

Built for the **[CubeForge SDK](https://github.com/Gildofj/cubeforge.sdk)** ecosystem and loaded natively by the **[CubeForge Loader](https://github.com/Gildofj/cubeforge.loader)**.

---

## ⚡ Highlights

- **Zero-Friction Setup**: Intelligent SDK resolution detects local SDK repositories (`../cubeforge.sdk`, `../CWSDK`) or downloads automatically via CMake `FetchContent`.
- **Modern Build Tooling**: Integrated `CMakePresets.json` (Release, Debug, RelWithDebInfo), 1-click PowerShell build scripts (`build.ps1` / `build.bat`), and automated deployment into your Cube World installation.
- **Rich Reference Implementation**: Includes clean, commented examples of in-game chat commands (`/ping`, `/help`, `/heal`, `/boost`, `/god`), character stat scaling (HP, Attack Power, Armor), DirectInput hotkeys (`F5`), frame ticks, and Dear ImGui overlays.
- **Battle-Tested Compiler Configuration**: Pre-configured with exact runtime definitions (`_ITERATOR_DEBUG_LEVEL=0`, `DIRECTINPUT_VERSION=0x0800`, `NOMINMAX`, `WIN32_LEAN_AND_MEAN`) to ensure 100% ABI compatibility with Cube World.
- **CI/CD Included**: Ready-to-use GitHub Actions workflow (`.github/workflows/build.yml`) that compiles and packages release artifacts on every push and release tag.

---

## 🚀 Quick Start in 3 Steps

### 1. Clone or Generate from Template

Click **"Use this template"** on GitHub, or clone locally:

```bash
git clone https://github.com/Gildofj/cubeforge.mod-template.git MyCubeWorldMod
cd MyCubeWorldMod
```

### 2. Compile the Mod

#### Option A: 1-Click PowerShell Build (Recommended)
```powershell
# Compiles the Release x64 DLL
.\build.ps1

# Or compile and automatically deploy to your Cube World installation:
.\build.ps1 -InstallPath "C:\Program Files (x86)\Steam\steamapps\common\Cube World"
```

#### Option B: Visual Studio 2022 / 2019
1. Open Visual Studio.
2. Select **File -> Open -> Folder...** and choose your cloned repository.
3. In the configuration dropdown, select **`windows-release`** (or `x64-Release`).
4. Press **`Ctrl + Shift + B`** (or **Build -> Build All**).

#### Option C: CMake CLI
```bash
# Configure & Build using CMake Presets
cmake --preset windows-release
cmake --build --preset windows-release
```

Compiled binaries are output to `dist/Mods/CubeForgeSampleMod.dll` and `dist/Mods/CubeForgeSampleMod.pdb`.

### 3. Play In-Game

1. Ensure **[CubeForge Loader](https://github.com/Gildofj/cubeforge.loader)** is installed in your Cube World root folder (`CubeWorld/CubeModLoader.fip`).
2. Copy `CubeForgeSampleMod.dll` into the `Mods/` directory:
   ```text
   C:\Program Files (x86)\Steam\steamapps\common\Cube World\Mods\CubeForgeSampleMod.dll
   ```
3. Launch Cube World via Steam.
4. Press `Enter` in-game and type `/ping` or `/help` to see your mod in action!

---

## 📂 Project Structure

```text
cubeforge.mod-template/
├── .github/
│   └── workflows/
│       └── build.yml               # Automated CI/CD GitHub Actions pipeline
├── docs/
│   ├── GETTING-STARTED.md          # Step-by-step beginner environment & setup guide
│   ├── MODDING-GUIDE.md            # Comprehensive cookbook with 8 practical recipes
│   ├── ARCHITECTURE.md             # In-depth look at DLL loading, VTables & memory
│   ├── TROUBLESHOOTING.md          # Common errors and their step-by-step solutions
│   └── CONTRIBUTING.md             # Code conventions and contribution guidelines
├── src/
│   ├── utils/
│   │   ├── ModUtils.h              # Chat printing, colors, string split & entity helpers
│   │   └── ModUtils.cpp            # Implementation of utility functions
│   ├── main.cpp                    # DLL entry point exporting MakeMod()
│   ├── SampleMod.h                 # Mod class inheriting GenericMod
│   └── SampleMod.cpp               # Chat commands, combat hooks, hotkeys & tick logic
├── CMakeLists.txt                  # Root CMake configuration (C++20, MSVC flags, SDK fetch)
├── CMakePresets.json               # Presets for Release, Debug, and RelWithDebInfo
├── build.ps1                       # Automated PowerShell build and installation script
├── build.bat                       # Windows batch build launcher
├── LICENSE                         # MIT License
└── README.md                       # Project overview and quick start guide
```

---

## 🛠️ Making the Mod Your Own

Customizing this template for your project takes less than a minute:

1. **Change the Project Name**:
   In [`CMakeLists.txt`](CMakeLists.txt), update:
   ```cmake
   project(MyAwesomeMod VERSION 1.0.0 LANGUAGES C CXX)
   set(MOD_TARGET_NAME "MyAwesomeMod")
   ```
2. **Rename Your Mod Class**:
   Rename `SampleMod.h` and `SampleMod.cpp` to `MyAwesomeMod.h` and `MyAwesomeMod.cpp`, and update the `MakeMod()` factory function in [`src/main.cpp`](src/main.cpp):
   ```cpp
   EXPORT GenericMod* MakeMod() {
       return new MyAwesomeMod();
   }
   ```
3. **Add Your Custom Logic**:
   Override events in `GenericMod` such as `OnChat`, `OnGameTick`, `OnCreatureAttackPowerCalculated`, `OnGetKeyboardState`, `OnZoneGenerated`, etc.

---

## 📖 Complete Documentation Index

| Guide | Description |
| :--- | :--- |
| 🚀 **[Getting Started](docs/GETTING-STARTED.md)** | Step-by-step setup guide for beginners (VS 2022, CMake, Loader). |
| 🍳 **[Modding Guide & Cookbook](docs/MODDING-GUIDE.md)** | 8 practical recipes (commands, combat scaling, items, hotkeys, UI). |
| 🏛️ **[Architecture Overview](docs/ARCHITECTURE.md)** | How Cube World modding works (DLL injection, VTables, memory layout). |
| 🔧 **[Troubleshooting Guide](docs/TROUBLESHOOTING.md)** | Solutions for common crashes, ABI mismatches, and build issues. |
| 🤝 **[Contributing Guidelines](docs/CONTRIBUTING.md)** | Clean Code, C++20 conventions, and contribution workflows. |

---

## 🤝 CubeForge Ecosystem

- 🏛️ **[CubeForge SDK](https://github.com/Gildofj/cubeforge.sdk)**: Complete Cube World data structures, offsets, and reverse-engineered types.
- 🚀 **[CubeForge Loader](https://github.com/Gildofj/cubeforge.loader)**: Native DLL mod loader and in-game UI engine.
- ⚡ **[CubeForge Mega Modpack](https://github.com/Gildofj/cubeforge.mega-modpack)**: Large-scale modular mod suite reference.
- 📈 **[CubeForge XP Progression](https://github.com/Gildofj/cubeforge.xp-progression)**: Clean Architecture reference mod featuring leveling and scaling.

---

## 📄 License

This template is open-source software licensed under the [MIT License](LICENSE). Feel free to use it as the foundation for any Cube World mod!
