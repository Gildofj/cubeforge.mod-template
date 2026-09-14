# Contributing to CubeForge Mod Template

Thank you for contributing to the **CubeForge Mod Template**! This document outlines our architectural standards, C++20 conventions, and pull request workflow.

---

## 🧭 Table of Contents

1. [Architectural Principles](#1-architectural-principles)
2. [C++20 Coding Standards](#2-c20-coding-standards)
3. [Repository Structure](#3-repository-structure)
4. [Development & Pull Request Workflow](#4-development--pull-request-workflow)
5. [Documentation Standards](#5-documentation-standards)

---

## 1. Architectural Principles

Our codebase adheres to strict engineering principles:

- **Do it right once, so you don't have to fix it twice**: Write modular, clean, and robust code.
- **Separation of Concerns**: Keep event interception, state management, and business logic decoupled.
- **RAII & Safety**: Manage Win32 handles, Direct3D resources, and Critical Sections using RAII guards (`CriticalSectionGuard`).
- **Defensive Programming**: Cube World's game state fluctuates between the main menu, loading screens, and gameplay. Always validate pointers (`cube::Game`, `cube::World`, `cube::Creature`) before accessing members.

---

## 2. C++20 Coding Standards

### Compiler Warnings
- Code must compile with **Zero Warnings** under MSVC Level 4 (`/W4`).
- Unused parameters in required hook signatures should be omitted or silenced via `(void)param;` / `/wd4100`.

### Naming Conventions
- **Classes & Structs**: `PascalCase` (e.g., `SampleMod`, `ModUtils`).
- **Methods & Functions**: `PascalCase` (e.g., `OnGameTick`, `PrintMessage`).
- **Member Variables**: `m_` prefix with `camelCase` (e.g., `m_damageBoostEnabled`, `m_tickCounter`).
- **Constants**: `PascalCase` or `k` prefix (e.g., `Colors::White`).
- **File Names**: Match primary class name (e.g., `SampleMod.h`, `SampleMod.cpp`).

### Modern C++ Features
- Use `auto` where types are obvious from context.
- Use `std::wstring` for wide text strings passed to and from Cube World.
- Prefer `constexpr` and `inline` constants over preprocessor `#define` macros.

---

## 3. Repository Structure

```text
cubeforge.mod-template/
├── .github/workflows/      # Automated CI/CD build actions
├── docs/                   # Developer documentation & cookbooks
├── src/
│   ├── utils/              # General-purpose reusable helpers
│   ├── main.cpp            # DLL exports (MakeMod)
│   ├── SampleMod.h         # Mod class header
│   └── SampleMod.cpp       # Mod implementation
├── CMakeLists.txt          # Root CMake build definition
├── CMakePresets.json       # CMake presets
├── build.ps1               # Automation script
└── README.md               # Main project overview
```

---

## 4. Development & Pull Request Workflow

1. **Fork and Clone**:
   ```bash
   git clone https://github.com/<your-username>/cubeforge.mod-template.git
   cd cubeforge.mod-template
   ```
2. **Create a Feature Branch**:
   ```bash
   git checkout -b feature/my-new-feature
   ```
3. **Build & Test**:
   ```powershell
   .\build.ps1 -BuildType Release
   ```
4. **Test in Cube World**:
   Deploy the DLL into your `Mods/` folder and verify stability in-game.
5. **Submit a Pull Request**:
   Describe your changes clearly, referencing any related issues or discussions.

---

## 5. Documentation Standards

- Keep documentation synchronized with code modifications.
- Place all guides inside `docs/` using `UPPER-KEBAB.md` naming.
- Ensure all markdown code snippets specify the correct language identifier (`cpp`, `cmake`, `powershell`, `bash`, `text`).
