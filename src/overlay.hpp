#pragma once

#include <X11/X.h>
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_X11
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <functional>

typedef void (*overlay_frame_callback)(class Overlay* overlay);

class Overlay {
public:
    Overlay();
    ~Overlay();

    GLFWwindow* window;
    Window x_window;

    void run(std::function<void(class Overlay* overlay)> callback);
};