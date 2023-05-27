#include "core/core.hpp"
#include <X11/extensions/XI2.h>
#include <cstdio>
#include <iostream>
#include "imgui/imgui.h"
#include "overlay.hpp"
#include <thread>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include <functional>

std::chrono::high_resolution_clock timer;
std::chrono::time_point<std::chrono::high_resolution_clock> last_tick;


int main(int argc, char** argv, char** envp) {
    Overlay overlay;

    bool is_overlay_open = false;
        
    auto key_thread = std::thread([&is_overlay_open, &overlay]() {
        Display* display = XOpenDisplay(getenv("DISPLAY"));
        Window root = DefaultRootWindow(display);
        XIEventMask eventMask;
        eventMask.deviceid = XIAllMasterDevices;
        eventMask.mask_len = XIMaskLen(XI_LASTEVENT);
        eventMask.mask = (unsigned char*)calloc(eventMask.mask_len, sizeof(char));
        XISetMask(eventMask.mask, XI_RawKeyPress);
        XISetMask(eventMask.mask, XI_RawKeyRelease);
        XISelectEvents(display, root, &eventMask, 1);
        XSync(display, true);
        free(eventMask.mask);

        int xiOpcode;
        int queryEvent, queryError;
        if (XQueryExtension(display, "XInputExtension", &xiOpcode, &queryEvent, &queryError) == false) {
            printf("X Input extension not available.\n");
            return 1;
        }
        
        XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);
        int group = 0;
        XkbStateRec state;
        XkbGetState(display, XkbUseCoreKbd, &state);
        group = state.group;

        XEvent event;
        XGenericEventCookie* cookie = &event.xcookie;

        while (true) {
            XNextEvent(display, &event);
            if (XGetEventData(display, cookie) && cookie->type == GenericEvent && cookie->extension == xiOpcode) {
                if (cookie->evtype == XI_RawKeyPress) {
                    auto key_event = (XIRawEvent*)cookie->data;
                    if (key_event->detail == 118) {
                        is_overlay_open = !is_overlay_open;
                        glfwSetWindowAttrib(overlay.window, GLFW_MOUSE_PASSTHROUGH, !is_overlay_open);
                        if (is_overlay_open) {
                            XRaiseWindow(display, overlay.x_window);
                        }
                    }
                }
            }
        }

        XCloseDisplay(display);
    });
    key_thread.detach();

    auto callback = [&is_overlay_open](Overlay* overlay) {
        if (is_overlay_open) {
            core::draw_menu();
        }
    };
    std::thread update_thread([&overlay, &callback]() {
        mem::pid = mem::get_pid_by_name("R5Apex.exe");
        if (mem::pid == 0) {
            std::cout << "Couldn't find R5Apex.exe" << std::endl;
            return 1;
        }
        core::init();

        while (true) {
            auto now = timer.now();
            float delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_tick).count();
            last_tick = now;

            core::tick(delta_time);
        }
    });
    update_thread.detach();
    overlay.run(callback);
    return 0;
}