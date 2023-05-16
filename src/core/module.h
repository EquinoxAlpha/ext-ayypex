#pragma once

#include <unordered_map>

class Module {
    public:

    bool active;

    virtual void tick(float delta_time) = 0;
    virtual void draw() = 0;
    virtual void init() = 0;

    Module() {
        active = true;
    }

    virtual ~Module() = default;
};