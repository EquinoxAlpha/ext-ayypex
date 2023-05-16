#pragma once

#include "../module.h"

class Glow : public Module {
    public:
    Glow() : Module() {
        active = true;
    }
    
    void tick(float delta_time) override;
    void draw() override;
    void init() override;
};