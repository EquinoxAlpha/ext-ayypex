#pragma once

#include "math.h"
#include <cmath>

// Prediction is broken
namespace prediction {
struct prediction_ctx {
        float launch_velocity, gravity;
        vec3 target_velocity;
        vec3 target_position, launch_position;
        vec3 angles;
        float time_of_flight, pitch, distance;
        bool success;
};

inline bool optimal(float x, float y, float v0, float g, float &pitch) {
    const float root = v0 * v0 * v0 * v0 - g * (g * x * x + 2.0f * y * v0 * v0);
    if (root < 0.0f) {
        return false;
    }
    pitch = atan((v0 * v0 - sqrt(root)) / (g * x));
    return true;
}

inline bool solve_trajectory(vec3 target, prediction_ctx &ctx) {
    vec3 v = ctx.launch_position - target;
    const float dx = sqrt(v.x * v.x + v.y * v.y);
    const float dy = v.z;
    const float v0 = ctx.launch_velocity;
    const float g = ctx.gravity;
    if (!optimal(dx, dy, v0, g, ctx.pitch)) {
        return false;
    }
    ctx.time_of_flight = dx / (cos(ctx.pitch) * v0);
    ctx.success = true;
    ctx.angles.x = ctx.pitch * TO_DEGREES;
    ctx.angles.y = -std::atan2(v.x, v.y) * TO_DEGREES - 90.0f;
    ctx.angles.z = 0.0f;
    ctx.angles = math::normalize_angles(ctx.angles);
    return true;
}

inline bool predict_trajectory(prediction_ctx &ctx) {
    static const float MAX_TIME = 1.0f;
    static const float TIME_STEP = 1.0 / 256.0f;

    if (ctx.launch_velocity <= 1.0f) {
        ctx.angles = math::calculate_angles(ctx.launch_position, ctx.target_position);
        ctx.time_of_flight = 0.0f;
        ctx.success = true;
        return true;
    }

    for (float target_time = 0.0f; target_time <= MAX_TIME; target_time += TIME_STEP) {
        const auto target_pos = vec3::extrapolate(ctx.target_position, ctx.target_velocity, target_time);

        if (!solve_trajectory(target_pos, ctx)) {
            return false;
        }
        if (ctx.time_of_flight < target_time) {
            return true;
        }
    }
    return false;
}
} // namespace prediction