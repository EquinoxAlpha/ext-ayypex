#pragma once

#include "logging.hpp"
#include "math.hpp"
#include "memory.hpp"
#include <math.h>
#include "../offsets.hpp"

namespace utils {
    inline float get_mouse_sensitivity() {
        float mouse_sensitivity;
        mem::read<float>(BASE + offsets::mouse_sensitivity + 0x68, mouse_sensitivity);
        return mouse_sensitivity * 0.022f;
    }

    inline float angle_to_pixels(float angle){
        float x = angle / get_mouse_sensitivity();
        return x;
    };

    inline vec2 angle_to_pixels(vec2 angles) {
        return vec2(angle_to_pixels(angles.x), angle_to_pixels(angles.y));
    }

    inline vec3 angle_to_pixels(vec3 angles) {
        return vec3(angle_to_pixels(angles.x), angle_to_pixels(angles.y), 0);
    }
}