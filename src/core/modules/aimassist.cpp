#include "aimassist.hpp"

#include "../common.hpp"
#include "../sdk/prediction.hpp"
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

using prediction::prediction_ctx;

Entity* AimAssist::target = nullptr;

void AimAssist::tick(float delta_time) {
    if (!globals.held_weapon || !globals.local_player)
        return;
    
    target = nullptr;

    float closest_distance = std::numeric_limits<float>::max();
    for (auto &[index, entity] : globals.entity_map) {
        if (!entity)
            continue;
        
        auto classname = Entity::get_class_name(entity->address);

        if (classname != "CPlayer" && classname != "CAI_BaseNPC")
            continue;
        Player* player = (Player*)entity;

        float distance = math::get_view_delta(globals.local_player->camera_position, player->get_bone(), globals.local_player->view_angles).magnitude();

        if (distance > settings.aim_fov)
            continue;
        if (distance < closest_distance) {
            closest_distance = distance;
            target = entity;
        }
    }

    if (!target)
        return;
    
    if (!globals.local_player->is_ads)
        return;
    
    bool is_teammate = target->team_id == globals.local_player->team_id;
    if (!target->is_visible || is_teammate) {
        return;
    }

    std::cout << "aiming at " << target << std::endl;

    prediction_ctx ctx;

    ctx.gravity = globals.held_weapon->projectile_gravity;
    ctx.target_velocity = target->velocity;
    ctx.target_position = ((Player*)(target))->get_bone(0);
    ctx.launch_position = globals.local_player->camera_position;
    ctx.launch_velocity = globals.held_weapon->projectile_speed;
    prediction::predict_trajectory(ctx);

    auto get_fov_scale = []() {
        if (globals.local_player->is_ads) {
            if (globals.held_weapon->zoom_fov != 0.0f && globals.held_weapon->zoom_fov != 1.0f) {
                return globals.held_weapon->zoom_fov / 90.0f;
            }
        }
        return 1.0f;
    };

    if (ctx.success) {
        std::cout << "ctx.angles: " << ctx.angles.to_string() << std::endl;
        float delta = (globals.local_player->view_angles - ctx.angles).magnitude();
        float fov_scale = get_fov_scale();
        if (delta < settings.aim_deadzone.x * fov_scale) {
            return;
        }
        
        const auto speed = std::log(settings.aim_strength + delta / (fov_scale * fov_scale) * settings.aim_strength) * settings.aim_strength + settings.aim_strength;
        auto delta_angles = ctx.angles - globals.local_player->view_angles;
        delta_angles = math::normalize_angles(delta_angles);

        auto mouse_angles = utils::angle_to_pixels(delta_angles);

        logger::info("mouse_angles: %f, %f", mouse_angles.x, mouse_angles.y);

        globals.mouse.move(mouse_angles.y, mouse_angles.x);
        globals.mouse.flush();
    }
}

void AimAssist::draw() {
}

void AimAssist::draw_menu() {
    if (ImGui::BeginTabItem("Aim Assist")) {
        ImGui::Checkbox("Enabled", &active);
        ImGui::SliderFloat("Strength", &settings.aim_strength, 0.0f, 1.0f);
        ImGui::SliderFloat("Deadzone", &settings.aim_deadzone.x, 0.0f, 3.0f);
        ImGui::SliderFloat("FOV", &settings.aim_fov, 0.0f, 180.0f);
        ImGui::EndTabItem();
    }
}

void AimAssist::init() {
}