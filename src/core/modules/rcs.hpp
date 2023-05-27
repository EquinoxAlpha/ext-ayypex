#pragma once

#include "module.hpp"
#include "../common.hpp"

class RecoilControl : public Module {
    public:
    RecoilControl() : Module() {
        active = true;
    }

    vec3 old_punch = vec3(0,0,0);
    
    void tick(float delta_time) override;
    void draw() override;
    void draw_menu() override;
    void init() override;
};