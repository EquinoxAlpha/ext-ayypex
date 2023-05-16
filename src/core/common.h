#pragma once

#include "configuration.h"
#include "sdk/math.h"
#include "sdk/draw_list.h"
#include "sdk/mem.h"
#include "mouse.h"
#include "sdk/utils.h"
#include "sdk/sdk.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

struct globals_t {
    vec2 window_size;
    Player* local_player;
    Weapon* held_weapon;
    std::vector<Entity*> entities;
    std::unordered_map<int, Entity*> entity_map;
    Mouse mouse;
};

inline globals_t globals;