#include "draw_list.hpp"
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <imgui/imgui.h>
#include <iostream>

DrawList::DrawList() {
    entries = std::unordered_map<std::string, std::vector<std::string>>();
}

DrawList::~DrawList() {
    entries.clear();
}

void DrawList::visualize(std::string name, const char* fmt, ...) {
    static std::hash<std::string> hasher;
    
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsprintf(buffer, fmt, args);
    va_end(args);

    if (entries.find(name) == entries.end()) {
        entries[name] = std::vector<std::string>();
    }

    entries[name].push_back(buffer);
}

void DrawList::begin() {
    entries.clear();
    window_size = ImGui::GetIO().DisplaySize;
}

void DrawList::end() {
    int start_x = 10;
    int start_y = 10;

    int title_text_size = 25;
    int log_text_size = 15;

    int padding = 5;
    int line_height = 5;
    
    ImDrawList* list = ImGui::GetBackgroundDrawList();
    ImFont* font = ImGui::GetFont();

    // Render all log categories and their messages. Adjust the start_x based on the longest title or log message. If the total width of the log category
    // exceeds the window width, then adjust start_y by the biggest log height. The title should be rendered above the log messages.
    int biggest_log_height = 0;
    for (auto& [title, logs] : entries) {
        ImVec2 title_size = font->CalcTextSizeA(title_text_size, FLT_MAX, 0.0f, title.c_str());

        // Get the biggest log message, and the total size of this log.
        ImVec2 total_log_size(title_size.x, title_size.y);
        for (auto& log : logs) {
            ImVec2 log_size = font->CalcTextSizeA(log_text_size, FLT_MAX, 0.0f, log.c_str());
            total_log_size.x = std::max(total_log_size.x, log_size.x);
            total_log_size.y += log_size.y + line_height;
        }
        //biggest_log_height = std::max(biggest_log_height, (int)total_log_size.y);

        // If the total log width exceeds the window width, then adjust start_y by the biggest log height.
        if (total_log_size.x + start_x > window_size.x) {
            start_y += biggest_log_height + padding;
            biggest_log_height = 0;
            start_x = 10;
        }

        biggest_log_height = std::max(biggest_log_height, (int)total_log_size.y);

        // Render the title.
        list->AddText(font, title_text_size, ImVec2(start_x, start_y), IM_COL32(255, 255, 255, 255), title.c_str());

        // Render the logs.
        int log_y = start_y + title_size.y + padding;
        for (auto& log : logs) {
            list->AddText(font, log_text_size, ImVec2(start_x, log_y), IM_COL32(255, 255, 255, 255), log.c_str());
            log_y += log_text_size + line_height;
        }

        // Adjust the start_x based on the biggest log width.
        start_x += total_log_size.x + padding;
    }
}


DrawList draw_list;