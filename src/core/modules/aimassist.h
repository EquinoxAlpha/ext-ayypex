#pragma once

#include "../module.h"
#include "../common.h"

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
    void init() override;
};