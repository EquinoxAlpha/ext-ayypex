#pragma once

#include "configuration.hpp"
#include "sdk/math.hpp"
#include "sdk/draw_list.hpp"
#include "sdk/memory.hpp"
#include "mouse.hpp"
#include "sdk/utils.hpp"
#include "sdk/sdk.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <imgui/imgui.h>

struct globals_t {
    vec2 window_size;
    Player* local_player;
    Weapon* held_weapon;
    std::vector<Entity*> entities;
    std::unordered_map<int, Entity*> entity_map;
    Mouse mouse;
};

inline globals_t globals;