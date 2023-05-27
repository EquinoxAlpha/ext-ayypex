#pragma once

#include "module.hpp"

class Glow : public Module {
    public:
    Glow() : Module() {
        active = true;
    }
    
    void tick(float delta_time) override;
    void draw() override;
    void draw_menu() override;
    void init() override;
};