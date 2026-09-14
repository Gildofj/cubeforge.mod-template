# Cube World Modding Troubleshooting Guide

Diagnose and resolve common build errors, runtime crashes, and injection issues encountered when developing mods with **CubeForge SDK**.

---

## 🧭 Table of Contents

1. [Build & Compiler Errors](#1-build--compiler-errors)
   - [Compiler not found in PATH (`cl.exe`)](#compiler-not-found-in-path-clexe)
   - [MinGW / GCC Linker conflict (`ld.exe`)](#mingw--gcc-linker-conflict-ldexe)
   - [Unresolved External Symbol (`LNK2019`)](#unresolved-external-symbol-lnk2019)
   - [`_ITERATOR_DEBUG_LEVEL` mismatch](#_iterator_debug_level-mismatch)
2. [Game Startup & Injection Errors](#2-game-startup--injection-errors)
   - [Immediate crash on game launch (`0xC0000005`)](#immediate-crash-on-game-launch-0xc0000005)
   - [Mod DLL is ignored / not loaded by the game](#mod-dll-is-ignored--not-loaded-by-the-game)
   - [Antivirus deletes or quarantines mod DLL](#antivirus-deletes-or-quarantines-mod-dll)
3. [Runtime Crashes & Logic Bugs](#3-runtime-crashes--logic-bugs)
   - [Null Pointer Crash when accessing player or world](#null-pointer-crash-when-accessing-player-or-world)
   - [Chat command echoes into public chat](#chat-command-echoes-into-public-chat)
4. [Live Debugging with Visual Studio](#4-live-debugging-with-visual-studio)

---

## 1. Build & Compiler Errors

### Compiler not found in PATH (`cl.exe`)
- **Symptom**: `CMake Error: The CMAKE_CXX_COMPILER: cl is not a full path and was not found in the PATH.`
- **Cause**: The terminal does not have the Visual Studio MSVC environment variables loaded.
- **Solution**:
  - Run the automated build script **`.\build.ps1`** (which discovers and loads `vcvars64.bat` automatically via `vswhere.exe`).
  - Or run your commands from the **x64 Native Tools Command Prompt for VS 2022**.

---

### MinGW / GCC Linker conflict (`ld.exe`)
- **Symptom**: `LINK Pass 1: command "C:\...\ld.exe" failed ... cannot find /nologo`
- **Cause**: MinGW/Scoop is installed and its `ld.exe` shadows MSVC's `link.exe`.
- **Solution**:
  - Run **`.\build.ps1`** — it ensures MSVC's toolchain has highest precedence in the environment `PATH`.
  - Ensure `CMakePresets.json` specifies `"CMAKE_LINKER": "link"`.

---

### Unresolved External Symbol (`LNK2019`)
- **Symptom**: `error LNK2019: unresolved external symbol ...`
- **Cause**: Missing source files or unlinked `CWSDK` library target.
- **Solution**: Ensure your target links against `CWSDK` in `CMakeLists.txt`:
  ```cmake
  target_link_libraries(MyMod PRIVATE CWSDK)
  ```

---

### `_ITERATOR_DEBUG_LEVEL` mismatch
- **Symptom**: Game crashes as soon as an `std::string` or `std::vector` is passed from your mod to the engine.
- **Cause**: The mod was built with standard Debug mode without disabling STL debug iterators.
- **Solution**: The template's `CMakeLists.txt` defines `_ITERATOR_DEBUG_LEVEL=0` and sets `MultiThreaded` CRT runtime. Always compile with the `windows-release` preset for gameplay.

---

## 2. Game Startup & Injection Errors

### Immediate crash on game launch (`0xC0000005`)
- **Cause 1**: The DLL was compiled for `x86` (32-bit) instead of `x64` (64-bit). Cube World is strictly a 64-bit application.
- **Cause 2**: Code inside `SampleMod::Initialize()` or global constructors attempted to dereference game memory pointers before the game engine finished initializing.
- **Solution**:
  - Verify your target architecture is `x64`.
  - Guard all pointer accesses with `nullptr` checks.
  - Defer world operations until `OnGameTick` or `OnZoneGenerated`.

---

### Mod DLL is ignored / not loaded by the game
- **Checklist**:
  1. Is `CubeModLoader.fip` (or `CubeForgeLoader.fip`) placed in the root Cube World directory alongside `Cube.exe`?
  2. Is your DLL inside the `Mods/` subfolder (`Cube World/Mods/MyMod.dll`)?
  3. Does your mod export `MakeMod()` with C linkage?
     ```cpp
     EXPORT GenericMod* MakeMod() {
         return new MyMod();
     }
     ```

---

### Antivirus deletes or quarantines mod DLL
- **Cause**: Because mod loaders inject native code into game processes, heuristic scanners may trigger a false positive.
- **Solution**: Add an exclusion for your Cube World directory and mod development workspace in Windows Defender / Antivirus settings.

---

## 3. Runtime Crashes & Logic Bugs

### Null Pointer Crash when accessing player or world
- **Symptom**: Crash to Desktop (CTD) during title screen or character select.
- **Cause**: `game->world` and `game->world->local_creature` are `nullptr` while on the main menu.
- **Solution**: Always validate pointers using `ModUtils::GetLocalPlayer()` or explicit guards:
  ```cpp
  cube::Game* game = cube::GetGame();
  if (!game || !game->world || !game->world->local_creature) {
      return; // Not in active gameplay yet
  }
  ```

---

### Chat command echoes into public chat
- **Symptom**: Typing `/ping` prints the response, but also broadcasts `/ping` into multiplayer chat.
- **Cause**: `OnChat` returned `0` instead of `1`.
- **Solution**: Return `1` to suppress the command from public chat broadcast:
  ```cpp
  int MyMod::OnChat(std::wstring* message) {
      if (message && *message == L"/ping") {
          ModUtils::PrintMessage(L"Pong!", ModUtils::Colors::Green);
          return 1; // 1 = Suppress broadcast
      }
      return 0; // 0 = Allow normal chat
  }
  ```

---

## 4. Live Debugging with Visual Studio

To set breakpoints and inspect live memory while Cube World is running:

1. Compile the mod using the **`windows-relwithdebinfo`** (or `windows-release`) preset to generate debug symbols (`.pdb`).
2. Copy both `MyMod.dll` and `MyMod.pdb` into the game's `Mods/` directory.
3. Launch Cube World via Steam.
4. In Visual Studio, go to **Debug -> Attach to Process...** (or press `Ctrl + Alt + P`).
5. Select **`Cube.exe`** from the process list and click **Attach**.
6. Set breakpoints inside your mod's source code (e.g., in `OnChat` or `OnCreatureAttackPowerCalculated`). Breakpoints will trigger seamlessly!
