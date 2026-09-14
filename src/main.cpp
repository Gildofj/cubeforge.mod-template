#include "cwsdk.h"
#include "SampleMod.h"

/**
 * @file main.cpp
 * @brief Dynamic-link library (DLL) entry point and mod instantiation for Cube World.
 *
 * When Cube World starts with CubeForge Loader (or CubeModLoader.fip), the loader:
 * 1. Scans the 'Mods/' folder for 64-bit DLLs.
 * 2. Calls the exported 'MakeMod()' factory function to instantiate your mod class.
 * 3. Registers your mod in the callback dispatcher table based on priority values.
 * 4. Calls your mod's 'Initialize()' method once game offsets are mapped into memory.
 */

/**
 * @brief Main Mod Factory Function exported to the CubeForge Loader.
 *
 * All mods must export this symbol with C linkage (extern "C" __declspec(dllexport)).
 * The CubeForge SDK macro EXPORT handles the calling convention and export attributes automatically.
 */
EXPORT GenericMod* MakeMod() {
    return new SampleMod();
}

/**
 * @note Versioning & Pre-Initialization:
 *
 * Default implementations of the following required exports are automatically provided
 * and exported by CWSDK:
 *   - EXPORT int ModMajorVersion();      // Defaults to CWSDK major version
 *   - EXPORT int ModMinorVersion();      // Defaults to CWSDK minor version
 *   - EXPORT void ModPreInitialize();    // Pre-initialization hook
 *
 * You do not need to implement them unless you wish to override default version checks.
 */
