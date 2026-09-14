#pragma once

#include "cwsdk.h"
#include <string>
#include <vector>

namespace ModUtils {

    // Common RGBA Color Palette for In-Game Chat / Messages
    namespace Colors {
        inline const FloatRGBA White(1.0f, 1.0f, 1.0f, 1.0f);
        inline const FloatRGBA Green(0.2f, 1.0f, 0.2f, 1.0f);
        inline const FloatRGBA Red(1.0f, 0.3f, 0.3f, 1.0f);
        inline const FloatRGBA Cyan(0.3f, 0.8f, 1.0f, 1.0f);
        inline const FloatRGBA Yellow(1.0f, 0.9f, 0.2f, 1.0f);
        inline const FloatRGBA Gold(1.0f, 0.84f, 0.0f, 1.0f);
        inline const FloatRGBA Orange(1.0f, 0.6f, 0.1f, 1.0f);
        inline const FloatRGBA Magenta(1.0f, 0.3f, 0.9f, 1.0f);
    }

    /**
     * @brief Safely prints a wide string message to the Cube World chat box.
     * @param message Wide string to print.
     * @param color Color of the message (defaults to White).
     */
    void PrintMessage(const std::wstring& message, const FloatRGBA& color = Colors::White);

    /**
     * @brief Safely prints a UTF-8 string message to the Cube World chat box.
     * @param utf8Message UTF-8 string to print.
     * @param color Color of the message (defaults to White).
     */
    void PrintMessage(const std::string& utf8Message, const FloatRGBA& color = Colors::White);

    /**
     * @brief Splits a wide string into tokens based on a delimiter.
     * @param str The string to split.
     * @param delimiter Delimiter character (defaults to space).
     * @return Vector of tokens.
     */
    std::vector<std::wstring> Split(const std::wstring& str, wchar_t delimiter = L' ');

    /**
     * @brief Retrieves the local player creature instance safely.
     * @return Pointer to local cube::Creature, or nullptr if not available.
     */
    cube::Creature* GetLocalPlayer();

    /**
     * @brief Checks if a given creature is the local player.
     * @param creature Pointer to creature to test.
     * @return true if creature is local player, false otherwise.
     */
    bool IsLocalPlayer(cube::Creature* creature);

    /**
     * @brief Safely adds health to a creature up to their maximum HP.
     * @param creature Pointer to the creature.
     * @param amount Health points to restore.
     */
    void HealCreature(cube::Creature* creature, float amount);

} // namespace ModUtils
