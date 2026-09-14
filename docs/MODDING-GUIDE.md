# CubeForge Modding Guide & Practical Cookbook

A comprehensive, developer-oriented cookbook for creating native C++20 mods with **CubeForge SDK**.

---

## 🧭 Table of Contents

1. [The `GenericMod` Event Architecture](#1-the-genericmod-event-architecture)
   - [Event Priorities](#event-priorities)
   - [Event Suppression vs Propagation](#event-suppression-vs-propagation)
2. [Recipe 1: Creating Custom Chat Commands with Arguments](#recipe-1-creating-custom-chat-commands-with-arguments)
3. [Recipe 2: Modifying Combat & Creature Stats (RPG Mechanics)](#recipe-2-modifying-combat--creature-stats-rpg-mechanics)
4. [Recipe 3: Listening to Custom Hotkeys & Input (DirectInput)](#recipe-3-listening-to-custom-hotkeys--input-directinput)
5. [Recipe 4: Creating In-Game UI Overlays (Dear ImGui)](#recipe-4-creating-in-game-ui-overlays-dear-imgui)
6. [Recipe 5: Intercepting Items, Equipment & Shop Prices](#recipe-5-intercepting-items-equipment--shop-prices)
7. [Recipe 6: Working with World Zones & Block Coordinates](#recipe-6-working-with-world-zones--block-coordinates)
8. [Recipe 7: Invoking Native Game Functions Safely](#recipe-7-invoking-native-game-functions-safely)
9. [Recipe 8: Thread Synchronization & Critical Sections](#recipe-8-thread-synchronization--critical-sections)

---

## 1. The `GenericMod` Event Architecture

All Cube World mods inherit from `GenericMod` (`#include "cwsdk.h"`). When the game engine executes an event, `CubeForge Loader` invokes the registered callback on each active mod.

### Event Priorities

Each event handler has an associated priority field configured in your mod's constructor:

```cpp
class MyMod : public GenericMod {
public:
    MyMod() {
        // Available priorities:
        // VeryHighPriority (0) -> First to execute
        // HighPriority     (1)
        // NormalPriority   (2) -> Default
        // LowPriority      (3)
        // VeryLowPriority  (4) -> Last to execute

        OnChatPriority = HighPriority;
        OnCreatureHPCalculatedPriority = NormalPriority;
    }
};
```

### Event Suppression vs Propagation

For callbacks that return an `int` (such as `OnChat`, `OnCheckInventoryFull`, `OnWindowProc`):
- Return **`1`**: Consumes / suppresses the event. The original game behavior and lower-priority mods will not process it.
- Return **`0`**: Allows normal execution and propagation to other mods.

---

## Recipe 1: Creating Custom Chat Commands with Arguments

### Goal
Create a command like `/teleport <x> <y> <z>` or `/gold <amount>` that intercepts user chat and provides colored feedback.

```cpp
#include "SampleMod.h"
#include "utils/ModUtils.h"

int MyMod::OnChat(std::wstring* message) {
    if (!message || message->empty()) return 0;

    // Check if message is a command
    if ((*message)[0] == L'/') {
        std::vector<std::wstring> tokens = ModUtils::Split(*message, L' ');
        const std::wstring& cmd = tokens[0];

        // Command: /gold <amount>
        if (cmd == L"/gold") {
            cube::Creature* player = ModUtils::GetLocalPlayer();
            if (player) {
                int amount = 1000;
                if (tokens.size() > 1) {
                    amount = std::stoi(tokens[1]);
                }
                player->gold += amount;
                ModUtils::PrintMessage(L"[CubeForge] Added " + std::to_wstring(amount) + L" gold coins!", ModUtils::Colors::Gold);
            }
            return 1; // Suppress message from appearing in game chat
        }
    }

    return 0; // Pass normal messages through
}
```

---

## Recipe 2: Modifying Combat & Creature Stats (RPG Mechanics)

### Goal
Scale health, damage, armor, and critical hit rate dynamically.

```cpp
void MyMod::OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) {
    if (!creature || !power) return;

    // Check if the target is the local player
    if (ModUtils::IsLocalPlayer(creature)) {
        // Player gets a 30% Attack Power boost
        *power *= 1.30f;
    } else if (creature->entity_data.hostile) {
        // Boss/enemy damage scaling (e.g. Nightmare difficulty mod)
        *power *= 1.50f;
    }
}

void MyMod::OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) {
    if (!creature || !critical) return;

    if (ModUtils::IsLocalPlayer(creature)) {
        // +10% Critical Strike Chance bonus
        *critical += 0.10f;
    }
}
```

---

## Recipe 3: Listening to Custom Hotkeys & Input (DirectInput)

### Goal
Listen to key combinations (e.g., `F5`, `Tab`, `Shift`) without freezing the game thread, using edge-trigger debouncing.

```cpp
void MyMod::OnGetKeyboardState(BYTE* diKeys) {
    if (!diKeys) return;

    // DirectInput scan code for F6 (DIK_F6)
    // 0x80 indicates that the key is currently pressed down
    bool isF6Down = (diKeys[DIK_F6] & 0x80) != 0;

    // Edge detection: Trigger only on key down transition (not every frame held)
    if (isF6Down && !m_f6WasPressed) {
        m_flightModeActive = !m_flightModeActive;
        ModUtils::PrintMessage(m_flightModeActive ? L"Flight Mode: ON" : L"Flight Mode: OFF", ModUtils::Colors::Cyan);
    }

    m_f6WasPressed = isF6Down;
}
```

---

## Recipe 4: Creating In-Game UI Overlays (Dear ImGui)

### Goal
Render custom UI windows inside Cube World's DirectX 11 render pipeline.

```cpp
void MyMod::OnDrawImGui() {
    if (!m_showConfigWindow) return;

    // When CubeForge Loader has Dear ImGui enabled:
    // ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);
    // if (ImGui::Begin("CubeForge Mod Settings", &m_showConfigWindow)) {
    //     ImGui::Text("Character Tuning");
    //     ImGui::SliderFloat("Damage Multiplier", &m_damageMultiplier, 1.0f, 5.0f, "%.2fx");
    //     ImGui::Checkbox("Invulnerability", &m_godMode);
    //     ImGui::End();
    // }
}
```

---

## Recipe 5: Intercepting Items, Equipment & Shop Prices

### Goal
Allow characters to equip weapons from any class or modify shop item costs.

```cpp
void MyMod::OnCreatureCanEquipItem(cube::Creature* creature, cube::Item* item, bool* equipable) {
    if (!creature || !item || !equipable) return;

    // Example: Allow Rogues to equip bows (Any-Weapon mod)
    if (creature->entity_data.classType == cube::Creature::ClassType::Rogue) {
        if (item->category == 3 /* Bow / Crossbow */) {
            *equipable = true;
        }
    }
}

void MyMod::OnGetItemBuyingPrice(cube::Item* item, int* price) {
    if (price && *price > 0) {
        // 20% discount at all vendors
        *price = static_cast<int>(*price * 0.80f);
    }
}
```

---

## Recipe 6: Working with World Zones & Block Coordinates

### Goal
React to newly generated zones and query world coordinates.

```cpp
void MyMod::OnZoneGenerated(cube::Zone* zone) {
    if (!zone) return;

    // Zone region coordinates (each region contains multiple chunks)
    int rx = zone->region_x;
    int ry = zone->region_y;

    // Log or spawn custom structures
    // ModUtils::PrintMessage(L"Entering Region (" + std::to_wstring(rx) + L", " + std::to_wstring(ry) + L")");
}
```

---

## Recipe 7: Invoking Native Game Functions Safely

### Goal
Call game engine methods (like playing sounds or spawning particles) using `Invoker.h`.

```cpp
#include "common/Invoker.h"

void PlayHitSound(cube::Game* game, FloatVector3 position) {
    // CallGameFunction<ReturnType>(relativeOffset, arguments...)
    // Example sound ID 0x0 (sound_hit)
    CallGameFunction<void>(0x0A0420, game, 0, &position, 1.0f, 1.0f, false);
}
```

---

## Recipe 8: Thread Synchronization & Critical Sections

### Goal
Prevent multi-threading crashes when accessing shared game resources (like active zones or creature lists).

```cpp
#include "common/CriticalSectionGuard.h"

void SafeIterateCreatures(cube::Game* game) {
    if (!game || !game->world) return;

    // RAII guard automatically enters and leaves the Win32 Critical Section
    CriticalSectionGuard lock(game->world->zones_critical_section);

    for (cube::Creature* creature : game->world->creatures) {
        if (creature) {
            // Safe thread-protected access to creature data
        }
    }
} // Critical section lock automatically released here
```

---

## 🎯 Next Steps

- Check **[Architecture Overview](ARCHITECTURE.md)** to understand the low-level mechanics of DLL injection and VTables.
- Check **[Troubleshooting Guide](TROUBLESHOOTING.md)** if you encounter crashes or compiler errors.
