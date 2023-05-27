#pragma once
#include "imgui/imgui.h"
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

class DrawList {
public:
    DrawList();
    ~DrawList();

    std::unordered_map<std::string, std::vector<std::string>> entries;
    ImVec2 window_size;

    void visualize(std::string name, const char* fmt, ...);

    void begin();
    void end();
};

extern DrawList draw_list;