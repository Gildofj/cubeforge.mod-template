#pragma once

#include "cwsdk.h"
#include "utils/ModUtils.h"
#include <string>
#include <vector>

/**
 * @brief SampleMod - Reference Mod Implementation for CubeForge SDK.
 *
 * This class inherits from GenericMod to receive game events dispatched by
 * the CubeForge Loader. Override any virtual methods to customize gameplay.
 */
class SampleMod : public GenericMod {
public:
    SampleMod();
    virtual ~SampleMod() = default;

    // -------------------------------------------------------------------------
    // 1. Mod Lifecycle
    // -------------------------------------------------------------------------

    /**
     * @brief Called immediately after the DLL is loaded and base address is resolved.
     * Use this method for memory scans, loading configuration files, or internal setup.
     */
    virtual void Initialize() override;

    // -------------------------------------------------------------------------
    // 2. Game Loop & Frame Updates
    // -------------------------------------------------------------------------

    /**
     * @brief Called every frame before the main game logic update.
     * @param game Pointer to root cube::Game instance.
     */
    virtual void OnGameTick(cube::Game* game) override;

    // -------------------------------------------------------------------------
    // 3. Chat & Commands
    // -------------------------------------------------------------------------

    /**
     * @brief Intercepts in-game chat messages.
     * @param message Pointer to wide string containing the chat text.
     * @return 1 to suppress the message (command handled), 0 to pass through to the game.
     */
    virtual int OnChat(std::wstring* message) override;

    // -------------------------------------------------------------------------
    // 4. Creature Stat Calculations
    // -------------------------------------------------------------------------

    /**
     * @brief Intercepts maximum HP calculations for creatures.
     * @param creature Pointer to the creature whose HP is being calculated.
     * @param hp Pointer to calculated HP. Modifying this changes maximum HP.
     */
    virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override;

    /**
     * @brief Intercepts Attack Power calculations.
     * @param creature Pointer to the creature.
     * @param power Pointer to calculated Attack Power.
     */
    virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override;

    /**
     * @brief Intercepts Armor calculations.
     * @param creature Pointer to the creature.
     * @param armor Pointer to calculated Armor.
     */
    virtual void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override;

    // -------------------------------------------------------------------------
    // 5. Input & Hotkeys
    // -------------------------------------------------------------------------

    /**
     * @brief Intercepts DirectInput keyboard state array.
     * @param diKeys Array of 256 bytes representing DirectInput key states (DIK_*).
     */
    virtual void OnGetKeyboardState(BYTE* diKeys) override;

    // -------------------------------------------------------------------------
    // 6. World & Zones
    // -------------------------------------------------------------------------

    /**
     * @brief Called whenever a new world zone finishes generation.
     * @param zone Pointer to the generated cube::Zone.
     */
    virtual void OnZoneGenerated(cube::Zone* zone) override;

    // -------------------------------------------------------------------------
    // 7. GUI / ImGui Overlay
    // -------------------------------------------------------------------------

    /**
     * @brief Called within the DirectX 11 / ImGui rendering frame.
     * Render custom debug windows, mod menus, or HUD overlays here.
     */
    virtual void OnDrawImGui() override;

private:
    // Internal state & configuration flags
    uint64_t m_tickCounter{0};
    bool m_damageBoostEnabled{false};
    bool m_godModeEnabled{false};
    float m_damageMultiplier{1.25f};

    // Hotkey edge-detection debounce state (for DirectInput)
    bool m_f5KeyWasPressed{false};
    bool m_showImGuiWindow{false};

    // Helper method to process chat commands
    bool ProcessCommand(const std::vector<std::wstring>& args);
};
