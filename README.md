# CubeForge Mod Template

[![C++20](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)](https://www.microsoft.com/windows)
[![Toolchain](https://img.shields.io/badge/Compiler-MSVC%20%2F%20Clang--cl%20(x64)-brightgreen.svg)](https://visualstudio.microsoft.com/)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C.svg)](https://cmake.org/)
[![CubeForge SDK](https://img.shields.io/badge/CubeForge%20SDK-v2.0.0-purple.svg)](https://github.com/Gildofj/cubeforge.sdk)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Official starter template and reference implementation for creating native C++20 mods for **Cube World** (Steam Release `1.0.0-1`).

Powered by the **[CubeForge SDK](https://github.com/Gildofj/cubeforge.sdk)** and loaded by the **[CubeForge Loader](https://github.com/Gildofj/cubeforge.loader)**.

---

## ⚡ Features Included in this Template

- **Zero Manual Setup**: Uses CMake `FetchContent` to download and configure the latest CubeForge SDK automatically.
- **Chat Command Interception**: Example handling for in-game commands like `/ping` and `/boost`.
- **Dynamic Stat Modification**: Demonstrates scaling character Attack Power and health safely during game stat recalculations.
- **Game Loop Hook**: `OnGameTick` callback for per-frame custom updates.
- **Automated CI/CD**: Pre-configured GitHub Actions workflow compiling your mod on every push.

---

## 🚀 Quick Start: Creating Your Mod

### 1. Use this Template

Click the green **"Use this template"** button at the top of this repository on GitHub to create your own mod repository.

Or clone it locally:

```bash
git clone https://github.com/Gildofj/cubeforge.mod-template.git MyCubeWorldMod
cd MyCubeWorldMod
```

### 2. Prerequisites

* **OS**: Windows 10 or 11 (64-bit).
* **IDE**: [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) (or 2019) with the **Desktop development with C++** and **C++ CMake tools for Windows** workloads.
* **Game**: Cube World (Steam Release 1.0.0-1).
* **Mod Loader**: [CubeForge Loader](https://github.com/Gildofj/cubeforge.loader).

### 3. Build with Visual Studio

1. Open Visual Studio.
2. Select **File -> Open -> Folder...** and choose your cloned mod directory.
3. In the top toolbar, ensure the configuration is set to **`x64-Release`** (or `x64-Clang-Release`).
   > [!WARNING]
   > **Never build in Debug mode (`_DEBUG`)**: Debug mode modifies STL iterator layouts, causing immediate memory desynchronization with Cube World's Release binary.
4. Press **`Ctrl + Shift + B`** (or select **Build -> Build All**).
5. The compiled `.dll` will be generated in `out/build/x64-Release/` (or `build/Release/`).

### 4. Build with Command Line (CMake)

```bash
# Configure
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release
```

### 5. Install & Test In-Game

1. Copy the compiled `CubeForgeSampleMod.dll` into your game's `Mods/` directory:
   ```text
   C:\Program Files (x86)\Steam\steamapps\common\Cube World\Mods\
   ```
2. Start Cube World via Steam with `CubeModLoader.fip` (or launch `CubeModLauncher.exe`).
3. Press `Enter` in-game and type `/ping` to see the mod respond in chat!

---

## 📂 Project Structure

```text
cubeforge.mod-template/
├── .github/
│   └── workflows/
│       └── build.yml       # GitHub Actions CI automated build workflow
├── src/
│   ├── main.cpp            # DLL exports (ModMajorVersion, MakeMod)
│   ├── SampleMod.h         # Mod class inheriting from GenericMod
│   └── SampleMod.cpp       # Mod logic, chat handlers, stat hooks
├── .gitignore              # Ignores build outputs and temp files
├── CMakeLists.txt          # CMake project definition with FetchContent
├── LICENSE                 # MIT License
└── README.md               # Getting started guide
```

---

## 🧠 Modding Concepts

### Event Priorities

Each event handler in `GenericMod` can define a priority:

```cpp
OnChatPriority = HighPriority; // VeryHigh, High, Normal, Low, VeryLow
```

### Chat Interception

Return `1` to consume/suppress the message, or `0` to let normal game processing continue:

```cpp
int SampleMod::OnChat(std::wstring* message) {
    if (*message == L"/ping") {
        cube::Game* game = (cube::Game*)CWOffset(0x0);
        if (game) {
            FloatRGBA green(0.2f, 1.0f, 0.2f, 1.0f);
            game->PrintMessage(L"Pong!", &green);
        }
        return 1; // Suppress
    }
    return 0; // Pass through
}
```

---

## 📚 Documentation & Ecosystem

- 📖 **[CubeForge SDK](https://github.com/Gildofj/cubeforge.sdk)**: API Reference, struct definitions, and memory offsets.
- 🚀 **[CubeForge Loader](https://github.com/Gildofj/cubeforge.loader)**: Mod loader engine and in-game UI.

---

## 📄 License

This template is released under the [MIT License](LICENSE).
