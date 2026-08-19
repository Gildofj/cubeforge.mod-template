#include "SampleMod.h"
#include <iostream>

SampleMod::SampleMod() {
    // Configure event callback priorities (VeryHigh = 0, High = 1, Normal = 2, Low = 3, VeryLow = 4)
    OnChatPriority = HighPriority;
    OnGameTickPriority = NormalPriority;
    OnCreatureHPCalculatedPriority = NormalPriority;
    OnCreatureAttackPowerCalculatedPriority = NormalPriority;
}

void SampleMod::Initialize() {
    // Safe initialization point for internal state or memory patches
}

void SampleMod::OnGameTick(cube::Game* game) {
    m_tickCount++;
}

int SampleMod::OnChat(std::wstring* message) {
    if (!message) return 0;

    // Example Command 1: /ping
    if (*message == L"/ping") {
        cube::Game* game = (cube::Game*)CWOffset(0x0);
        if (game) {
            FloatRGBA green(0.2f, 1.0f, 0.2f, 1.0f);
            game->PrintMessage(L"[CubeForge] Pong! Mod template is working.", &green);
        }
        return 1; // Suppress original chat message broadcast
    }

    // Example Command 2: /boost
    if (*message == L"/boost") {
        m_damageBoostEnabled = !m_damageBoostEnabled;
        cube::Game* game = (cube::Game*)CWOffset(0x0);
        if (game) {
            FloatRGBA cyan(0.3f, 0.8f, 1.0f, 1.0f);
            std::wstring status = m_damageBoostEnabled ? L"[CubeForge] Damage Boost: ON (+20%)" : L"[CubeForge] Damage Boost: OFF";
            game->PrintMessage(status.c_str(), &cyan);
        }
        return 1;
    }

    return 0; // Normal chat behavior
}

void SampleMod::OnCreatureHPCalculated(cube::Creature* creature, float* hp) {
    // Optional: Scale creature health
    // if (hp) *hp *= 1.0f;
}

void SampleMod::OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) {
    if (m_damageBoostEnabled && power) {
        *power *= 1.20f; // +20% Attack Power boost
    }
}
