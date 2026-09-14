#include "ModUtils.h"
#include <sstream>
#include <windows.h>

namespace ModUtils {

    void PrintMessage(const std::wstring& message, const FloatRGBA& color) {
        cube::Game* game = cube::GetGame();
        if (game) {
            // Note: PrintMessage expects a non-const pointer to FloatRGBA in the SDK definition
            FloatRGBA clr = color;
            game->PrintMessage(message.c_str(), &clr);
        }
    }

    void PrintMessage(const std::string& utf8Message, const FloatRGBA& color) {
        if (utf8Message.empty()) return;
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8Message.c_str(), (int)utf8Message.size(), NULL, 0);
        std::wstring wstr(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8Message.c_str(), (int)utf8Message.size(), &wstr[0], size_needed);
        PrintMessage(wstr, color);
    }

    std::vector<std::wstring> Split(const std::wstring& str, wchar_t delimiter) {
        std::vector<std::wstring> tokens;
        std::wstringstream ss(str);
        std::wstring item;
        while (std::getline(ss, item, delimiter)) {
            if (!item.empty()) {
                tokens.push_back(item);
            }
        }
        return tokens;
    }

    cube::Creature* GetLocalPlayer() {
        cube::Game* game = cube::GetGame();
        if (!game || !game->world) return nullptr;
        return game->world->local_creature;
    }

    bool IsLocalPlayer(cube::Creature* creature) {
        if (!creature) return false;
        cube::Creature* player = GetLocalPlayer();
        return (creature == player);
    }

    void HealCreature(cube::Creature* creature, float amount) {
        if (!creature) return;
        creature->entity_data.HP += amount;
        // In Cube World, creature's max HP is calculated dynamically or stored in entity data
        // Prevent health from dropping below 0
        if (creature->entity_data.HP < 0.0f) {
            creature->entity_data.HP = 0.0f;
        }
    }

} // namespace ModUtils
