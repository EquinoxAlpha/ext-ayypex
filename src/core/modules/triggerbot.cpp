#include "triggerbot.hpp"

#include "../common.hpp"
#include "imgui/imgui.h"
#include <iostream>
#include <string>

void Triggerbot::tick(float delta_time) {
    //if (!globals.held_weapon || !globals.held_weapon->is_valid) return;

    static float last_shot_time = 0.0f;
    static bool was_autofiring = false;
    
    for (auto &[index, entity] : globals.entity_map) {
        if (entity->signifier != PLAYER && entity->signifier != NPC_DUMMIE) continue;

        Player* player = (Player*)entity;

        //player->update();

        if (player->team_id == globals.local_player->team_id) continue;


        if (player->get_last_crosshair_time() + 0.3f > Game::get_current_time()) {
            goto attack;
        }
    }

    if (was_autofiring) {
        globals.mouse.release();
        globals.mouse.flush();
        was_autofiring = false;
    }

    return;

    attack:
    // assume the weapon is full-auto
    was_autofiring = true;
    globals.mouse.press();
    globals.mouse.flush();
    //last_shot_time = Game::get_current_time();
    /*if (globals.held_weapon->ready_to_fire) {
        globals.mouse.click();
        last_shot_time = Game::get_current_time();
    }*/
    /*if (globals.held_weapon->is_semi_auto) {
        if (globals.held_weapon->ready_to_fire) {
            globals.mouse.click();
            last_shot_time = Game::get_current_time();
        }
    } else {
        was_autofiring = true;
        globals.mouse.press();
        globals.mouse.flush();
    }*/
}

void Triggerbot::draw() {
}

void Triggerbot::draw_menu() {
    if (ImGui::BeginTabItem("Triggerbot")) {
        ImGui::Checkbox("Enabled", &active);
        ImGui::EndTabItem();
    }
}

void Triggerbot::init() {
}