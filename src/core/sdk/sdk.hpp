#pragma once

#include "logging.hpp"
#include "../offsets.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <sys/types.h>
#include "math.hpp"
#include "memory.hpp"

#define NUM_ENTS 0x10000
#define BASE 0x140000000

#define PLAYER 0x726579616c70
#define PROP_SURVIVAL 0x7275735f706f7270
#define NPC_DUMMIE 0x6d6d75645f63706e

struct CEntInfo {
        uintptr_t pEntity;
        int64_t SerialNumber;
        uintptr_t pPrev;
        uintptr_t pNext;
};

struct CEntityList {
    CEntInfo entities[NUM_ENTS];
};

struct glow_mode {
    uint8_t border_mode, general_mode, border_size, unk;
};

class Game {
    public:
    inline static float get_current_time() {
        float time;
        mem::read<float>(BASE + offsets::globals + 0x28, time);
        return time;
    }

    inline static float get_tick_interval() {
        float interval;
        mem::read<float>(BASE + offsets::globals + 0x30, interval);
        return interval;
    }
};

class Entity {
    public:
    Entity() : address(0) {};
    Entity(uintptr_t address) : address(address) {}
    virtual ~Entity() = default;
    
    uintptr_t address;
    bool is_valid;
    uintptr_t signifier;
    int shield_health;
    int max_shield_health;
    int team_id;
    int squad_id;
    int script_int;
    bool is_visible;
    bool is_alive;
    bool is_knocked;
    vec3 position;
    vec3 velocity;
    vec3 collision_max;
    vec3 collision_min;

    inline virtual void update() {
        if (!address) {
            this->invalidate();
            return;
        }
        uintptr_t signifier_ptr;
        mem::read<uintptr_t>(address + offsets::entity::m_iName, signifier_ptr);
        if (!signifier_ptr) {
            this->invalidate();
            return;
        }

        //TODO: use mem::read_multiple for even faster updates

        int life_state;
        int bleedout_state;
        mem::read_multiple<int>(std::vector<std::pair<uintptr_t, int*>>({
            std::make_pair(address + offsets::entity::m_shieldHealth, &shield_health),
            std::make_pair(address + offsets::entity::m_shieldHealthMax, &max_shield_health),
            std::make_pair(address + offsets::entity::m_iTeamNum, &team_id),
            std::make_pair(address + offsets::entity::m_squadID, &squad_id),
            std::make_pair(address + offsets::entity::m_customScriptInt, &script_int),
            std::make_pair(address + offsets::entity::m_lifeState, &life_state),
            std::make_pair(address + offsets::entity::m_bleedoutState, &bleedout_state)
        }));
        is_alive = !(life_state > 0);
        is_knocked = bleedout_state > 0;

        float last_visible_time;

        mem::read<uintptr_t>(signifier_ptr, signifier);
        /*mem::read<int>(address + offsets::entity::m_shieldHealth, shield_health);
        mem::read<int>(address + offsets::entity::m_shieldHealthMax, max_shield_health);
        mem::read<int>(address + offsets::entity::m_iTeamNum, team_id);
        mem::read<int>(address + offsets::entity::m_squadID, squad_id);
        mem::read<int>(address + offsets::entity::m_customScriptInt, script_int);*/
        mem::read<float>(address + offsets::lastVisibleTime, last_visible_time);
        is_visible = last_visible_time > Game::get_current_time() - 0.1f;
        /*mem::read<vec3>(address + offsets::entity::m_vecAbsOrigin, position);
        mem::read<vec3>(address + offsets::entity::m_vecAbsVelocity, velocity);
        mem::read<vec3>(address + offsets::entity::collision_max, collision_max);
        mem::read<vec3>(address + offsets::entity::collision_min, collision_min);*/

        mem::read_multiple(std::vector<std::pair<uintptr_t, vec3*>>({
            std::make_pair(address + offsets::entity::m_vecAbsOrigin, &position),
            std::make_pair(address + offsets::entity::m_vecAbsVelocity, &velocity),
            std::make_pair(address + offsets::entity::collision_max, &collision_max),
            std::make_pair(address + offsets::entity::collision_min, &collision_min)
        }));

        is_valid = true;
    }

    inline vec3 get_position_now() {
        vec3 pos;
        mem::read<vec3>(address + offsets::entity::m_vecAbsOrigin, pos);
        return pos;
    }

    inline void set_address(uintptr_t addr) {
        this->address = addr;
    }

    inline void invalidate() {
        address = 0;
        is_valid = false;
    }

    // Offsets are broken
    // TODO: get offsets from offsets.h instead of hardcoding them
    inline void enable_glow() {
        int glow_enabled = false;
        mem::read<int>(address + 0x03c8, glow_enabled);
        if (!glow_enabled) {
            mem::write<int>(address + 0x03c8, 1);
        }
        
        int glow_context = 0;
        mem::read<int>(address + 0x03d0, glow_context);
        if (glow_context != 2) {
            mem::write<int>(address + 0x03d0, 2);
        }

        const glow_mode default_mode = {101,102,49,75};
        int default_mode_int = *(int*)&default_mode;
        int mode; mem::read<int>(address + 0x02c4, mode);
        if (mode != default_mode_int) {
            mem::write<glow_mode>(address + 0x02c4, default_mode);
        }
    }

    inline void set_glow_color(vec3 color) {
        vec3 current_color; mem::read<vec3>(address + 0x1d0, current_color);
        if (current_color == color) return;
        mem::write<vec3>(address + 0x1d0, color);
    }

    static std::string get_class_name(uintptr_t pointer) {
        uintptr_t client_networkable_vtable;
        uintptr_t get_client_entity;
        uint32_t offset;
        uintptr_t network_name_ptr;
        mem::read<uintptr_t>(pointer + 3 * 8, client_networkable_vtable);
        mem::read<uintptr_t>(client_networkable_vtable + 3 * 8, get_client_entity);
        mem::read<uint32_t>(get_client_entity + 3, offset);
        mem::read<uintptr_t>(get_client_entity + offset + 7 + 16, network_name_ptr);
        char buffer[32];
        mem::read_array<char>(network_name_ptr, buffer, 32);
        std::string result;
        size_t len;
        for (len = 0; len < 32; ++len)
            if (buffer[len] == '\0')
                break;
        result.assign(buffer, len);
        return result;
    }
};

class Weapon : public Entity {
    public:
    Weapon() : Entity() {};
    Weapon(uintptr_t address) : Entity(address) {};
    virtual ~Weapon() = default;

    float projectile_speed;
    float projectile_gravity_scale;
    float projectile_gravity;
    float zoom_fov;
    bool is_semi_auto;
    bool ready_to_fire;

    inline virtual void update() {
        Entity::update();
        if (!address || !signifier) {
            this->invalidate();
            return;
        }
        mem::read<float>(address + offsets::weapon::projectile_launch_speed, projectile_speed);
        mem::read<float>(address + offsets::weapon::projectile_gravity_scale, projectile_gravity_scale);
        projectile_gravity = projectile_gravity_scale * 750.0f;
        mem::read<float>(address + offsets::weapon::m_targetZoomFOV, zoom_fov);
        mem::read<bool>(address + offsets::weapon::is_semi_auto, is_semi_auto);

        float current_time = Game::get_current_time();
        float interval = Game::get_tick_interval();
        float next_ready_time; mem::read<float>(address + 0x1648, next_ready_time);
        float next_primary_time; mem::read<float>(address + 0x164c, next_primary_time);
        float ready_time = next_ready_time > next_primary_time ? next_ready_time : next_primary_time;
        ready_to_fire = current_time >= ready_time - interval;
    }
};

struct kbutton_t {
    int down[2];
    int state;
};

struct Button {
    uintptr_t address;
    kbutton_t button;
    bool state;
    bool force;
    bool press;
    bool release;

    Button(uintptr_t address) : address(address), force(false), press(false), release(false) {
    }

    inline void update() {
        mem::read<kbutton_t>(address, button);
        state = (button.state & 1) != 0;
    }

    inline void post() {
        if (force) {
            int state;
            if (press && !release) {
                state = 5;
            }
            else if (!press && release) {
                state = 4;
            }
            else  {
                state = button.down[0] == 0 && button.down[1] == 0 ? 4 : 5;
            }

            if (state != button.state) {
                mem::write<int>(address + 8, state);
            }
            force = false;
        }
    }
};

inline Button InAttack(BASE + offsets::in_attack);

class Player : public Entity {
    public:
    Player() : Entity() {};
    Player(uintptr_t address) : Entity(address) {};
    virtual ~Player() = default;

    vec3 camera_position;
    vec3 view_angles;
    vec3 view_punch;
    
    bool is_ads;
    int held_weapon_index;
    int health;
    int max_health;
    float last_crosshair_time;

    inline virtual void update() {
        Entity::update();

        mem::read<vec3>(address + offsets::entity::camera_origin, camera_position);
        mem::read<vec3>(address + offsets::entity::m_vecPunchWeapon_Angle, view_punch);
        mem::read<vec3>(address + offsets::entity::view_angles, view_angles);
        mem::read<bool>(address + offsets::entity::m_bZooming, is_ads);
        mem::read<int>(address + offsets::entity::m_latestPrimaryWeapons, held_weapon_index);
        held_weapon_index &= 0xFFF;
        mem::read<int>(address + offsets::entity::m_iHealth, health);
        mem::read<int>(address + offsets::entity::m_iMaxHealth, max_health);
        mem::read<float>(address + offsets::lastCrosshairTime, last_crosshair_time);
    }

    inline float get_last_crosshair_time() {
        mem::read<float>(address + offsets::lastCrosshairTime, last_crosshair_time);
        return last_crosshair_time;
    }


    inline vec3 get_view_punch_now() {
        vec3 result;
        mem::read<vec3>(address + offsets::entity::m_vecPunchWeapon_Angle, result);
        return result;
    }

    inline vec3 get_sway_angles() {
        vec3 result;
        mem::read<vec3>(address + offsets::entity::view_angles, result);
        return result - get_view_punch_now();
    }

    inline uint16_t get_bone_id(const uint32_t &hitbox) {
        uintptr_t model; mem::read<uintptr_t>(address + offsets::studiohdr, model);
        if (!model) return 0;

        uintptr_t studiohdr; mem::read<uintptr_t>(model + 0x8, studiohdr);
        if (!studiohdr) return 0;

        uint16_t hitbox_cache; mem::read<uint16_t>(studiohdr + 0x34, hitbox_cache);
        if (!hitbox_cache) return 0;

        auto hitbox_array = studiohdr + ((uint16_t)(hitbox_cache & 0xFFFE) << (4 * (hitbox_cache & 1)));
        if (!hitbox_array) return 0;

        uint16_t index_cache; mem::read<uint16_t>(hitbox_array + 4, index_cache);
        if (!index_cache) return 0;

        auto hitbox_index = ((uint16_t)(index_cache & 0xFFFE) << (4 * (index_cache & 1)));
        if (!hitbox_index) return 0;

        uint16_t bone; mem::read<uint16_t>(hitbox_index + hitbox_array + (hitbox * 0x20), bone);
        return bone;
    }
    
    inline vec3 get_bone(int hitbox = 0) {
        uintptr_t bone_array; mem::read<uintptr_t>(address + offsets::entity::bone_array, bone_array);
        if (!bone_array) return this->position;

        int bone_id = get_bone_id(hitbox);
        if (!bone_id) return this->position;

        mat3x4 bone; mem::read<mat3x4>(bone_array + (bone_id * sizeof(mat3x4)), bone);
        return bone.translation() + this->position;
    }

    inline void set_angles(vec3 ang) {
        mem::write<vec3>(address + offsets::entity::view_angles, ang);
    }
};

namespace utils {
inline bool get_box_coords(Entity* entity, vec4& out) {
    if (!entity) return false;
    vec3 coords[8];

    vec3 min = entity->collision_max;
    vec3 max = entity->collision_min;
    vec3 origin = entity->get_position_now();

    coords[0] = min;
    coords[1] = max;
    coords[2] = vec3(min.x, min.y, max.z);
    coords[3] = vec3(min.x, max.y, min.z);
    coords[4] = vec3(min.x, max.y, max.z);
    coords[5] = vec3(max.x, min.y, min.z);
    coords[6] = vec3(max.x, min.y, max.z);
    coords[7] = vec3(max.x, max.y, min.z);

    vec2 screen_coords[8];

    for (int i = 0; i < 8; i++) {
        if (!math::world_to_screen(coords[i] + origin, screen_coords[i]))
            return false;
    }

    vec4 box = {1920, 1080, 0, 0};

    for (int i = 0; i < 8; i++) {
        if (screen_coords[i].x < box.x)
            box.x = screen_coords[i].x;
        if (screen_coords[i].y < box.y)
            box.y = screen_coords[i].y;
        if (screen_coords[i].x > box.z)
            box.z = screen_coords[i].x;
        if (screen_coords[i].y > box.w)
            box.w = screen_coords[i].y;
    }
    
    out = box;
    return true;
}
}