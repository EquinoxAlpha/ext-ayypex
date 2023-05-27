#include "glow.hpp"

#include "../common.hpp"
#include "imgui/imgui.h"

void Glow::tick(float delta_time) {
}

void Glow::draw() {
    for (auto& entity : globals.entities) {
        if (!entity) continue;

        const auto is_teammate = entity->team_id == globals.local_player->team_id;
        const auto color = is_teammate ? settings.glow_friendly_color : settings.glow_enemy_color;

        entity->enable_glow();
        if (entity->is_visible && !is_teammate) {
            entity->set_glow_color(vec3(0,0,15.0f));
        } else { 
            entity->set_glow_color(color); 
        }
    }
}

void Glow::draw_menu() {
    if (ImGui::BeginTabItem("Glow")) {
        ImGui::ColorEdit3("Enemy Color", &settings.glow_enemy_color.x);
        ImGui::ColorEdit3("Friendly Color", &settings.glow_friendly_color.x);
        ImGui::EndTabItem();
    }
}

void Glow::init() {
}