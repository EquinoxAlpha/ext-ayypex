#include "rcs.hpp"

#include "../common.hpp"
#include <cmath>
#include <iostream>
#include "../configuration.hpp"
#include "../sdk/utils.hpp"

void RecoilControl::tick(float delta_time) {
    static vec2 m;

    if (!globals.local_player || !globals.local_player->is_valid)
        return;

    vec3 punch = globals.local_player->get_view_punch_now();

    float pitch = std::max(old_punch.x - punch.x, 0.0f);
    float yaw = punch.y - old_punch.y;
    
    old_punch = punch;

    vec2 delta = utils::angle_to_pixels(vec2(yaw, pitch));
    m.x += delta.x;
    m.y += delta.y;

    int mx = std::round(m.x);
    int my = std::round(m.y);
    m.x -= mx;
    m.y -= my;
    globals.mouse.move(mx, my);
    globals.mouse.flush();
}

void RecoilControl::draw() {
}

void RecoilControl::draw_menu() {
    if (ImGui::BeginTabItem("Recoil Control")) {
        ImGui::Checkbox("Enabled", &active);
        ImGui::EndTabItem();
    }
}

void RecoilControl::init() {
}