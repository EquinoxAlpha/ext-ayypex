#pragma once

#include "../module.h"

class SpectatorCount : public Module {
    public:
    SpectatorCount() : Module() {
        active = true;
    }
    
    void tick(float delta_time) override;
    void draw() override;
    void init() override;
};