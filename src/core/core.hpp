#pragma once

#include "modules/module.hpp"
#include "common.hpp"
#include <mutex>
#include <vector>

namespace core {
    extern std::vector<Module*> modules;
    extern std::vector<CEntInfo> ent_infos;
    extern std::vector<CEntInfo> prev_infos;
    extern std::chrono::high_resolution_clock timer;
    extern std::chrono::high_resolution_clock::time_point last_tick;

    void update_entities();

    void tick(float delta_time);
    void draw();
    void draw_menu();
    void init();
}