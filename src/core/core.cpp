#include "core.hpp"

#include "imgui/imgui.h"
#include "modules/aimassist.hpp"
#include "modules/glow.hpp"
#include "modules/rcs.hpp"
#include "modules/triggerbot.hpp"
#include "offsets.hpp"
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include "common.hpp"
#include "sdk/memory.hpp"
#include "sdk/utils.hpp"

namespace core {
    std::vector<Module*> modules;
    std::vector<CEntInfo> ent_infos;
    std::vector<CEntInfo> prev_infos;
    std::chrono::high_resolution_clock timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_tick;

    void update_entities() {
        short local_player_handle;
        uintptr_t cworld;
        mem::read<short>(BASE + offsets::LocalPlayerHandle, local_player_handle);
        mem::read<uintptr_t>(BASE + offsets::cl_entitylist, cworld);
        if (local_player_handle == -1 || cworld == 0) {
            return;
        }

        std::swap(ent_infos, prev_infos);

        mem::read_array<CEntInfo>(BASE + offsets::cl_entitylist, ent_infos.data(), NUM_ENTS);

        globals.local_player->set_address(ent_infos[local_player_handle].pEntity);
        globals.local_player->update();

        if (globals.local_player->held_weapon_index != 0xFFF) {
            logger::debug("held weapon idx: %d", globals.local_player->held_weapon_index);
            logger::debug("held weapon addr: 0x%X", ent_infos[globals.local_player->held_weapon_index].pEntity);
            globals.held_weapon->set_address(ent_infos[globals.local_player->held_weapon_index].pEntity);
            logger::debug("Held weapon classname: %s", Entity::get_class_name(globals.held_weapon->address).c_str());
            globals.held_weapon->update();
        } else {
            globals.held_weapon->invalidate();
        }

        for (int i = 0; i < NUM_ENTS; i++) {
            auto& info = ent_infos[i];
            auto& prev_info = prev_infos[i];

            if (info.pEntity == prev_info.pEntity) {
                if (globals.entity_map.find(i) != globals.entity_map.end()) {
                    globals.entity_map[i]->update();
                }
            } else if (info.pEntity != 0 && i != local_player_handle) {
                if (globals.entity_map.find(i) != globals.entity_map.end()) {
                    delete globals.entity_map[i];
                    globals.entity_map.erase(i);
                }

                Entity* entity = nullptr;
                auto classname = Entity::get_class_name(info.pEntity);

                if (classname == "CPlayer" || classname == "CAI_BaseNPC") {
                    entity = new Player(info.pEntity);
                }

                if (entity) {
                    entity->update();
                    globals.entity_map[i] = entity;
                }
            } else {
                if (globals.entity_map.find(i) != globals.entity_map.end()) {
                    delete globals.entity_map[i];
                    globals.entity_map.erase(i);
                }
            }
        }
    }

    void tick(float delta_time) {
        short local_player_handle;
        mem::read<short>(BASE + offsets::LocalPlayerHandle, local_player_handle);
        if (!globals.local_player || local_player_handle == -1) return;

        //for (auto &[index, entity] : globals.entity_map) {
        //    entity->update();
        //}

        //globals.local_player->update();
        //globals.held_weapon->update();

        for (auto& module : modules) {
            if (module->active) {
                module->tick(delta_time);
            }
        }
        
        globals.mouse.flush();
    }

    void draw() {
        short local_player_handle;
        mem::read<short>(BASE + offsets::LocalPlayerHandle, local_player_handle);
        if (!globals.local_player || local_player_handle == -1) return;

        for (auto& module : modules) {
            if (module->active) {
                module->draw();
            }
        }
    }

    void draw_menu() {
        ImGui::Begin("##window");
        ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None);
        for (auto& module : modules) {
            module->draw_menu();
        }
        if (ImGui::BeginTabItem("Misc.")) {
            static bool show_demo_window = false;
            ImGui::Checkbox("Show ImGui Demo", &show_demo_window);
            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window);
            }
            static bool mat_fullbright = false;
            ImGui::Checkbox("mat_fullbright 1", &mat_fullbright);
            if (mat_fullbright) {
                mem::write<bool>(BASE + 0x01584c80+0x6c, mat_fullbright);
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
        ImGui::End();
    }

    void init() {
        logger::info("Initializing");

        globals.local_player = new Player();
        globals.held_weapon = new Weapon();
        
        for (int i = 0; i < NUM_ENTS; ++i) {
            ent_infos.push_back(CEntInfo());
            prev_infos.push_back(CEntInfo());
        }

        globals.entity_map.reserve(NUM_ENTS);

        std::thread entity_updater([]() {
            while (true) {
                update_entities();
            }
        });

        entity_updater.detach();

        modules.push_back(new AimAssist());
        modules.push_back(new Glow());
        modules.push_back(new RecoilControl());
        modules.push_back(new Triggerbot());

        //mat_fullbright 1
        //mem::write<bool>(BASE + 0x01584c80+0x6c, true);

        for (auto& module : modules) {
            module->init();
        }

        logger::info("Initialized %d modules", modules.size());
        
        globals.mouse.init();
        settings.init();
    }
}