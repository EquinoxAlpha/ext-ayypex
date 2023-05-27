#pragma once
#include "memory.hpp"
#include "../offsets.hpp"
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>
#include <sys/types.h>

inline constexpr float PI = 3.1415926535897932f;
inline constexpr float TO_RADIANS = PI / 180;
inline constexpr float TO_DEGREES = 180 / PI;

struct vec2 {
        float x, y;

        vec2() : x(0), y(0) {}
        vec2(float x, float y) : x(x), y(y) {}

        vec2 operator+(const vec2 &other) const { return vec2(x + other.x, y + other.y); }

        vec2 operator-(const vec2 &other) const { return vec2(x - other.x, y - other.y); }

        vec2 operator*(float scalar) const { return vec2(x * scalar, y * scalar); }

        vec2 operator/(float scalar) const { return vec2(x / scalar, y / scalar); }

        bool is_nan() const { return std::isnan(x) || std::isnan(y); }

        std::string to_string() const {
            std::ostringstream oss;
            oss << x << "," << y;
            return oss.str();
        }

        float magnitude() const { return std::sqrt(x * x + y * y); }

        vec2 normalize() const {
            float m = magnitude();
            return vec2(x / m, y / m);
        }

        static float distance(const vec2 &a, const vec2 &b) {
            vec2 diff = a - b;
            return diff.magnitude();
        }

        //ImVec2 to_imgui() { return {x, y}; }
};

struct vec3 {
        float x, y, z;

        // Constructors
        vec3() : x(0), y(0), z(0) {}

        vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        vec3(float x) : x(x), y(x), z(x) {}

        vec3 operator-(vec3 a) const { return {x - a.x, y - a.y, z - a.z}; }

        vec3 operator-(float a) const { return {x - a, y - a, z - a}; }

        vec3 operator+(vec3 a) const { return {x + a.x, y + a.y, z + a.z}; }

        vec3 operator+(float a) const { return {x + a, y + a, z + a}; }

        vec3 operator/(vec3 a) const { return {x / a.x, y / a.y, z / a.z}; }

        vec3 operator/(float a) const { return {x / a, y / a, z / a}; }

        vec3 operator*(vec3 a) const { return {x * a.x, y * a.y, z * a.z}; }

        vec3 operator*(float a) const { return {x * a, y * a, z * a}; }

        bool operator==(vec3 a) { return x == a.x && y == a.y && z == a.z; }

        inline float magnitude() { return std::sqrt(x * x + y * y + z * z); }

        inline vec3 normalize() {
            float length = this->magnitude();
            return {x / length, y / length, z / length};
        }

        inline static float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

        inline static vec3 cross(vec3 a, vec3 b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

        inline static float distance(vec3 a, vec3 b) { return vec3{a - b}.magnitude(); }

        inline std::string to_string() { return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z); }

        bool is_valid() { return !std::isnan(x) || !std::isnan(y) || !std::isnan(z); }

        inline static vec3 extrapolate(vec3 a, vec3 b, float time) { return a + (b * time); }

        inline static vec3 lerp(vec3 a, vec3 b, float t) { return a + (b - a) * t; }

        inline vec2 xy() { return vec2(x, y); }
};

struct mat4 {
        union {
                struct {
                        float _11;
                        float _12;
                        float _13;
                        float _14;
                        float _21;
                        float _22;
                        float _23;
                        float _24;
                        float _31;
                        float _32;
                        float _33;
                        float _34;
                        float _41;
                        float _42;
                        float _43;
                        float _44;
                };
                float m[4][4];
                float mm[16];
        };
};

struct mat3x4 {
        union {
                struct {
                        float _11;
                        float _12;
                        float _13;
                        float _14;
                        float _21;
                        float _22;
                        float _23;
                        float _24;
                        float _31;
                        float _32;
                        float _33;
                        float _34;
                };
                float m[3][4];
                float mm[12];
        };

        vec3 translation() { return {_14, _24, _34}; }
};

struct vec4 {
        float x, y, z, w;

        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        vec4 operator+(const vec4 &other) const { return vec4(x + other.x, y + other.y, z + other.z, w + other.w); }

        vec4 operator-(const vec4 &other) const { return vec4(x - other.x, y - other.y, z - other.z, w - other.w); }

        vec4 operator*(float scalar) const { return vec4(x * scalar, y * scalar, z * scalar, w * scalar); }

        vec4 operator/(float scalar) const { return vec4(x / scalar, y / scalar, z / scalar, w / scalar); }

        bool is_nan() const { return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w); }

        std::string to_string() const {
            std::ostringstream oss;
            oss << x << "," << y << "," << z << "," << w;
            return oss.str();
        }

        float magnitude() const { return std::sqrt(x * x + y * y + z * z + w * w); }

        vec4 normalize() const {
            float m = magnitude();
            return vec4(x / m, y / m, z / m, w / m);
        }

        static float distance(const vec4 &a, const vec4 &b) {
            vec4 diff = a - b;
            return diff.magnitude();
        }

        //ImVec4 to_imgui() { return {x, y, z, w}; }

        vec3 xyz() { return {x, y, z}; }

        vec2 xy() { return {x, y}; }

        vec2 zw() { return {z, w}; }

        vec4 transform(mat4& matrix) {
            vec4 result;
            result.x = x * matrix._11 + y * matrix._12 + z * matrix._13 + w * matrix._14;
            result.y = x * matrix._21 + y * matrix._22 + z * matrix._23 + w * matrix._24;
            result.z = x * matrix._31 + y * matrix._32 + z * matrix._33 + w * matrix._34;
            result.w = x * matrix._41 + y * matrix._42 + z * matrix._43 + w * matrix._44;
            return result;
        }
};

namespace math {
inline vec3 normalize_angles(vec3 angle) {
    while (angle.x > 89.0f)
        angle.x -= 180.f;

    while (angle.x < -89.0f)
        angle.x += 180.f;

    while (angle.y > 180.f)
        angle.y -= 360.f;

    while (angle.y < -180.f) {
        angle.y += 360.f;
    }

    angle.z = 0.0f;
    return angle;
}

inline static vec3 calculate_angles(vec3 src, vec3 dst) {
    vec3 delta = src - dst;
    vec3 angles;
    angles.x = std::asin(delta.z / delta.magnitude()) * 57.295779513082f;
    angles.y = -std::atan2(delta.x, delta.y) * 57.295779513082f - 90.0f;
    angles.z = 0.0f;
    return normalize_angles(angles);
}

// I don't know a better name for this function
inline static vec3 get_view_delta(vec3 src, vec3 dst, vec3 angles) {
    return calculate_angles(src, dst) - angles;
}

inline static bool world_to_screen(vec3 position, vec2 &out) {
    uintptr_t viewrenderer; mem::read<uintptr_t>(BASE + offsets::viewrenderer, viewrenderer);
    if (!viewrenderer)
        return false;

    uintptr_t matrix_ptr; mem::read<uintptr_t>(viewrenderer + offsets::viewmatrix, matrix_ptr);
    if (!matrix_ptr)
        return false;

    mat4 matrix; mem::read<mat4>(matrix_ptr, matrix);
    vec4 clip = vec4(position.x, position.y, position.z, 1.0f).transform(matrix);

    if (clip.w < 0.01f)
        return false;

    clip.x = clip.x * (1 / clip.w);
    clip.y = clip.y * (1 / clip.w);

    vec2 half_screen_size = {1920.f / 2, 1080.f / 2};

    out.x = half_screen_size.x + clip.x * half_screen_size.x;
    out.y = half_screen_size.y - clip.y * half_screen_size.y;

    return true;
}
}; // namespace math
