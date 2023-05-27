#pragma once

#include "module.hpp"
#include "../common.hpp"

class AimAssist : public Module {
    public:
    static Entity* target;

    AimAssist() : Module() {
        active = true;
    }
    
    static void silent_aim();
    static void before_createmove();
    static void after_createmove();
    void tick(float delta_time) override;
    void draw() override;
    void draw_menu() override;
    void init() override;
};