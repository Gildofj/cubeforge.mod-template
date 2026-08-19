#pragma once

#include "cwsdk.h"
#include <string>

/**
 * @brief SampleMod demonstrating key features of the CubeForge SDK.
 *
 * Inherits from GenericMod to receive lifecycle events, stat calculations,
 * and game hooks dispatched by the CubeForge Loader.
 */
class SampleMod : public GenericMod {
public:
    SampleMod();
    virtual ~SampleMod() = default;

    /**
     * @brief Called after mod loading and base address resolution.
     */
    virtual void Initialize() override;

    /**
     * @brief Called every frame prior to the main game logic update.
     * @param game Pointer to the root cube::Game instance.
     */
    virtual void OnGameTick(cube::Game* game) override;

    /**
     * @brief Intercepts in-game chat messages.
     * @param message Wide string containing the chat text.
     * @return 0 to allow the message, 1 to suppress/cancel original message broadcast.
     */
    virtual int OnChat(std::wstring* message) override;

    /**
     * @brief Intercepts max health calculation for all creatures.
     * @param creature Pointer to the cube::Creature being calculated.
     * @param hp Pointer to the computed HP value. Modifying this adjusts actual health.
     */
    virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override;

    /**
     * @brief Intercepts attack power calculation.
     * @param creature Pointer to the cube::Creature.
     * @param power Pointer to the computed attack power value.
     */
    virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override;

private:
    uint64_t m_tickCount{0};
    bool m_damageBoostEnabled{true};
};
