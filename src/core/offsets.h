#pragma once
#include <cstddef>
#include <cstdint>

#define BASE 0x140000000

namespace offsets {
    inline constexpr ptrdiff_t in_attack = 0x074034b0;
    inline constexpr ptrdiff_t LocalPlayerHandle = 0x1582728;
    inline constexpr ptrdiff_t local_player = 0x02139480 + 0x8; // .?AVC_GameMovement@@ + 0x8

    inline constexpr ptrdiff_t viewrenderer = 0x7400bd0;
    inline constexpr ptrdiff_t viewmatrix = 0x11a350;
    
    inline constexpr ptrdiff_t studiohdr = 0x10f0;

    inline constexpr ptrdiff_t cl_entitylist = 0x1d87e88;         //cl_entitylist

    inline constexpr ptrdiff_t globals = 0x165b880;             //GlobalVars

    inline constexpr ptrdiff_t lastVisibleTime = 0x1A78+0x10;       //lastVisibleTime

    inline constexpr ptrdiff_t mouse_sensitivity = 0x02125920;

    namespace entity {
        constexpr ptrdiff_t m_iHealth = 0x43c;                  //m_iHealth
        constexpr ptrdiff_t m_shieldHealth = 0x170;             //m_shieldHealth
        constexpr ptrdiff_t m_shieldHealthMax = 0x174;          //m_shieldHealthMax
        constexpr ptrdiff_t m_iMaxHealth = 0x0578;              //m_iMaxHealth
        constexpr ptrdiff_t bone_array = 0x0e98 + 0x48;         //m_nForceBone + 0x48
        constexpr ptrdiff_t collision_min = 0x04c0 + 0x10;      //m_Collision + 0x10
        constexpr ptrdiff_t collision_max = 0x04c0 + 0x1c;      //m_Collision + 0x1c
        constexpr ptrdiff_t m_vecAbsOrigin = 0x14c;             //m_vecAbsOrigin
        constexpr ptrdiff_t m_vecAbsVelocity = 0x0140;          //m_vecAbsVelocity
        constexpr ptrdiff_t m_iName = 0x0580;                   //m_iSignifierName
        constexpr ptrdiff_t m_latestPrimaryWeapons = 0x1a24;    //m_latestPrimaryWeapons
        constexpr ptrdiff_t camera_origin = 0x1f58;     
        constexpr ptrdiff_t m_ammoPoolCapacity = 0x25bc;        //m_ammoPoolCapacity
        constexpr ptrdiff_t view_angles = m_ammoPoolCapacity - 0x14;
        constexpr ptrdiff_t sway_angles = m_ammoPoolCapacity - 0x14 + 0x10;
        constexpr ptrdiff_t m_bZooming = 0x1c61;                //m_bZooming
        constexpr ptrdiff_t m_customScriptInt = 0x1648;         //m_customScriptInt
        constexpr ptrdiff_t m_iTeamNum = 0x044c;                //m_iTeamNum
        constexpr ptrdiff_t m_squadID = 0x0458;                 //m_squadID
        constexpr ptrdiff_t m_vecPunchWeapon_Angle = 0x24c0;    //m_vecPunchWeapon_Angle
        constexpr ptrdiff_t m_lifeState = 0x0798;               //m_lifeState
        constexpr ptrdiff_t m_bleedoutState = 0x2750;           //m_bleedoutState
    }

    namespace weapon {
        constexpr ptrdiff_t projectile_launch_speed = 0x1f50;          
        constexpr ptrdiff_t projectile_gravity_scale = 0x1f58;
        constexpr ptrdiff_t is_semi_auto = 0x1a88+0x18c;         //WeaponSettings.base + is_semi_auto
        constexpr ptrdiff_t m_targetZoomFOV = 0xbc;                 //m_targetZoomFOV
    }
}