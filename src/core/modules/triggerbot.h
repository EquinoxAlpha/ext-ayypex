#pragma once

#include "../module.h"

class Triggerbot : public Module {
    public:
    Triggerbot() : Module() {
        active = true;
    }
    
    void tick(float delta_time) override;
    void draw() override;
    void init() override;
};