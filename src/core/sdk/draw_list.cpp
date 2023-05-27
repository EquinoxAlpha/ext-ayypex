#include "draw_list.hpp"

#include "math.h"

void DrawList::add_line(vec2 start, vec2 end, vec3 color) {
}

void DrawList::add_circle(vec2 center, float radius, vec3 color) {
}

void DrawList::add_circle_filled(vec2 center, float radius, vec3 color) {
}

void DrawList::add_rect(vec2 start, vec2 end, vec3 color) {
}

void DrawList::add_rect(vec2 start, vec2 end, vec4 color) {
}

void DrawList::add_rect_filled(vec2 start, vec2 end, vec3 color) {
}

void DrawList::add_rect_filled(vec2 start, vec2 end, vec4 color) {
}

void DrawList::add_text(vec2 pos, const char* text, vec3 color) {
}

void DrawList::add_text_centered(vec2 pos, const char* text, vec3 color) {
}

// x_space and y_space are percentages of how much of the rectangle should be empty
void DrawList::add_rect_corners(vec2 start, vec2 end, float x_space, float y_space, vec3 color) {
    vec2 size = end - start;

    vec3 outline_color = vec3(0,0,0);

    vec2 x_space_size = vec2(size.x * x_space / 2, 0);
    vec2 y_space_size = vec2(0, size.y * y_space / 2);

    vec2 corner1(start.x, start.y);
    vec2 corner2(end.x, start.y);
    vec2 corner3(start.x, end.y);
    vec2 corner4(end.x, end.y);

    vec2 x = vec2(1, 0);
    vec2 y = vec2(0, 1);

    line_width = 2;

    add_line(corner1 - x, corner1 + x_space_size + x, outline_color);
    add_line(corner1 - y, corner1 + y_space_size + y, outline_color);

    add_line(corner2 + x, corner2 - x_space_size - x, outline_color);
    add_line(corner2 - y, corner2 + y_space_size + y, outline_color);

    add_line(corner3 - x, corner3 + x_space_size + x, outline_color);
    add_line(corner3 + y, corner3 - y_space_size - y, outline_color);
    
    add_line(corner4 + x, corner4 - x_space_size - x, outline_color);
    add_line(corner4 + y, corner4 - y_space_size - y, outline_color);

    line_width /= 2;

    add_line(corner1, corner1 + x_space_size, color);
    add_line(corner1, corner1 + y_space_size, color);

    add_line(corner2, corner2 - x_space_size, color);
    add_line(corner2, corner2 + y_space_size, color);

    add_line(corner3, corner3 + x_space_size, color);
    add_line(corner3, corner3 - y_space_size, color);
    
    add_line(corner4, corner4 - x_space_size, color);
    add_line(corner4, corner4 - y_space_size, color);
}

DrawList draw_list;