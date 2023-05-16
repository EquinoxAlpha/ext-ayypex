#pragma once

#include "sdk/math.h"
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include "sdk/logging.h"

using json = nlohmann::json;

class configuration {
    json settings;
    public:

    void init();
    void reload(std::string path);

    float aim_strength = 12.0f;
    vec2 aim_deadzone = vec2(0.3f, 0.3f);
    float aim_fov = 10.0f;
    int aim_hitbox = 1;

    vec3 esp_friendly_color = vec3(0.0f, 1.0f, 0.0f);
    vec3 esp_enemy_color = vec3(1.0f, 0.0f, 0.0f);
    float esp_thickness = 1.0f;
    bool esp_outline = true;

    vec3 glow_friendly_color = vec3(0.0f, 1.0f, 0.0f);
    vec3 glow_enemy_color = vec3(1.0f, 0.0f, 0.0f);
    bool glow_visible_only = true;

    vec2 recoil_control_strength = vec2(0.7f, 0.5f);

    template<typename T>
    inline void try_get(std::string key, T& value) {
        if (settings.find(key) != settings.end()) {
            value = settings[key];
        } else {
            logger::warn("Failed to find key %s in config file", key.c_str());
        }
    }
};

extern configuration settings;