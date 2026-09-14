#include "SampleMod.h"
#include <iostream>
#include <string>

SampleMod::SampleMod() {
    // -------------------------------------------------------------------------
    // Priority Configuration
    // Available priorities: VeryHighPriority (0), HighPriority (1), NormalPriority (2), LowPriority (3), VeryLowPriority (4)
    // -------------------------------------------------------------------------
    OnChatPriority = HighPriority;                         // Run early to intercept custom /commands
    OnGameTickPriority = NormalPriority;
    OnCreatureHPCalculatedPriority = NormalPriority;
    OnCreatureAttackPowerCalculatedPriority = NormalPriority;
    OnCreatureArmorCalculatedPriority = NormalPriority;
    OnGetKeyboardStatePriority = NormalPriority;
    OnZoneGeneratedPriority = NormalPriority;
    OnDrawImGuiPriority = NormalPriority;
}

void SampleMod::Initialize() {
    // This is called right after base addresses and offsets are ready.
    // Perfect place to load mod configs, initialize subsystem states, or perform memory patches.
}

void SampleMod::OnGameTick(cube::Game* game) {
    m_tickCounter++;

    // Passive regeneration effect while God Mode is active
    if (m_godModeEnabled && (m_tickCounter % 60 == 0)) {
        cube::Creature* player = ModUtils::GetLocalPlayer();
        if (player) {
            ModUtils::HealCreature(player, 50.0f);
        }
    }
}

int SampleMod::OnChat(std::wstring* message) {
    if (!message || message->empty()) {
        return 0;
    }

    // Commands must begin with '/'
    if ((*message)[0] != L'/') {
        return 0; // Allow normal chat to proceed
    }

    // Split command name and arguments by whitespace
    std::vector<std::wstring> args = ModUtils::Split(*message, L' ');
    if (args.empty()) {
        return 0;
    }

    bool handled = ProcessCommand(args);
    return handled ? 1 : 0; // 1 = Suppress message from public game chat; 0 = Allow pass-through
}

bool SampleMod::ProcessCommand(const std::vector<std::wstring>& args) {
    const std::wstring& cmd = args[0];

    // Command: /ping
    if (cmd == L"/ping") {
        ModUtils::PrintMessage(L"[CubeForge] Pong! Mod template is running smoothly.", ModUtils::Colors::Green);
        return true;
    }

    // Command: /help
    if (cmd == L"/help" || cmd == L"/cwsdk") {
        ModUtils::PrintMessage(L"--- [CubeForge Mod Template Commands] ---", ModUtils::Colors::Cyan);
        ModUtils::PrintMessage(L" /ping        - Tests mod responsiveness", ModUtils::Colors::White);
        ModUtils::PrintMessage(L" /boost       - Toggles +25% Attack Power", ModUtils::Colors::White);
        ModUtils::PrintMessage(L" /heal [amt]  - Restores HP (default: 100)", ModUtils::Colors::White);
        ModUtils::PrintMessage(L" /god         - Toggles invincible God Mode", ModUtils::Colors::White);
        ModUtils::PrintMessage(L" [F5 Hotkey]  - Toggles Mod Menu Overlay", ModUtils::Colors::Yellow);
        return true;
    }

    // Command: /boost
    if (cmd == L"/boost") {
        m_damageBoostEnabled = !m_damageBoostEnabled;
        if (m_damageBoostEnabled) {
            ModUtils::PrintMessage(L"[CubeForge] Attack Power Boost: ENABLED (+25%)", ModUtils::Colors::Gold);
        } else {
            ModUtils::PrintMessage(L"[CubeForge] Attack Power Boost: DISABLED", ModUtils::Colors::Orange);
        }
        return true;
    }

    // Command: /heal [amount]
    if (cmd == L"/heal") {
        cube::Creature* player = ModUtils::GetLocalPlayer();
        if (!player) {
            ModUtils::PrintMessage(L"[CubeForge] Player creature not found.", ModUtils::Colors::Red);
            return true;
        }

        float healAmount = 100.0f;
        if (args.size() > 1) {
            try {
                healAmount = std::stof(args[1]);
            } catch (...) {
                healAmount = 100.0f;
            }
        }

        ModUtils::HealCreature(player, healAmount);
        ModUtils::PrintMessage(L"[CubeForge] Healed for " + std::to_wstring((int)healAmount) + L" HP!", ModUtils::Colors::Green);
        return true;
    }

    // Command: /god
    if (cmd == L"/god") {
        m_godModeEnabled = !m_godModeEnabled;
        if (m_godModeEnabled) {
            ModUtils::PrintMessage(L"[CubeForge] God Mode: ACTIVATED (Max Armor + Regeneration)", ModUtils::Colors::Cyan);
        } else {
            ModUtils::PrintMessage(L"[CubeForge] God Mode: DEACTIVATED", ModUtils::Colors::Orange);
        }
        return true;
    }

    // Unrecognized slash command: let other mods or base game handle it
    return false;
}

void SampleMod::OnCreatureHPCalculated(cube::Creature* creature, float* hp) {
    if (!hp || !creature) return;

    // Example: Only apply modifications to the local player, leaving NPCs unmodified
    if (m_godModeEnabled && ModUtils::IsLocalPlayer(creature)) {
        *hp *= 5.0f; // 5x Health multiplier
    }
}

void SampleMod::OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) {
    if (!power || !creature) return;

    if (m_damageBoostEnabled && ModUtils::IsLocalPlayer(creature)) {
        *power *= m_damageMultiplier;
    }
}

void SampleMod::OnCreatureArmorCalculated(cube::Creature* creature, float* armor) {
    if (!armor || !creature) return;

    if (m_godModeEnabled && ModUtils::IsLocalPlayer(creature)) {
        *armor += 10000.0f; // Massive armor boost
    }
}

void SampleMod::OnGetKeyboardState(BYTE* diKeys) {
    if (!diKeys) return;

    // Detect F5 key press (0x80 means key is currently down)
    bool f5CurrentlyPressed = (diKeys[DIK_F5] & 0x80) != 0;

    // Edge trigger: Only fire when transitioning from not pressed -> pressed
    if (f5CurrentlyPressed && !m_f5KeyWasPressed) {
        m_showImGuiWindow = !m_showImGuiWindow;
        if (m_showImGuiWindow) {
            ModUtils::PrintMessage(L"[CubeForge] Mod Menu toggled ON", ModUtils::Colors::Cyan);
        } else {
            ModUtils::PrintMessage(L"[CubeForge] Mod Menu toggled OFF", ModUtils::Colors::Orange);
        }
    }

    m_f5KeyWasPressed = f5CurrentlyPressed;
}

void SampleMod::OnZoneGenerated(cube::Zone* zone) {
    if (!zone) return;
    // Example: Trigger world events or populate custom entities when a new zone is generated
}

void SampleMod::OnDrawImGui() {
    // If the mod loader has ImGui enabled, custom windows can be rendered here
    // Example (pseudo-code if Dear ImGui is included):
    // if (m_showImGuiWindow) {
    //     ImGui::Begin("CubeForge Mod Settings", &m_showImGuiWindow);
    //     ImGui::Checkbox("Damage Boost (+25%)", &m_damageBoostEnabled);
    //     ImGui::Checkbox("God Mode", &m_godModeEnabled);
    //     ImGui::End();
    // }
}
