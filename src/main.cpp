#include "cwsdk.h"
#include "SampleMod.h"

/**
 * @brief Exports the main mod factory function for the CubeForge Loader.
 *
 * Default versions (ModMajorVersion, ModMinorVersion) and early initialization (ModPreInitialize)
 * are provided automatically by the CubeForge SDK.
 */
EXPORT GenericMod* MakeMod() {
    return new SampleMod();
}
