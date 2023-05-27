#pragma once

#include "math.hpp"

class DrawList {
    public:

    void add_line(vec2 start, vec2 end, vec3 color = vec3(1, 1, 1));
    void add_circle(vec2 center, float radius, vec3 color = vec3(1, 1, 1));
    void add_circle_filled(vec2 center, float radius, vec3 color = vec3(1, 1, 1));
    void add_rect(vec2 start, vec2 end, vec3 color = vec3(1, 1, 1));
    void add_rect(vec2 start, vec2 end, vec4 color = vec4(1, 1, 1, 1));
    void add_rect_filled(vec2 start, vec2 end, vec3 color = vec3(1, 1, 1));
    void add_rect_filled(vec2 start, vec2 end, vec4 color);
    void add_text(vec2 pos, const char* text, vec3 color = vec3(1, 1, 1));
    void add_text_centered(vec2 pos, const char* text, vec3 color = vec3(1, 1, 1));

    void add_rect_corners(vec2 start, vec2 end, float x_space, float y_space, vec3 color = vec3(1, 1, 1));

    private:
    float line_width = 1.0f;
};

extern DrawList draw_list;