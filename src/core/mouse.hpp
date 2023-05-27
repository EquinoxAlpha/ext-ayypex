#pragma once

#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "sdk/logging.hpp"

class Mouse {
    int fd = -1;
    struct input_event event, event_end;
    public:
    inline void init() {
        std::ifstream devices("/proc/bus/input/devices");
        std::string line;
        while (std::getline(devices, line)) {
            if (line.find("mouse0") != std::string::npos && line.find("H: Handlers=") == 0) {
                std::string handler = line.substr(12);
                std::string event = "/dev/input/" + handler.substr(0, handler.find(" "));

                logger::info("Found mouse at %s", event.c_str());
                
                fd = open(event.c_str(), O_RDWR);
            }
        }
    }

    inline void move(int x, int y) {
        memset(&event, 0, sizeof(event));
        event.type = EV_REL;
        if (x != 0) {
            event.code = REL_X;
            event.value = x;
            write(fd, &event, sizeof(event));
        }
        if (y != 0) {
            event.code = REL_Y;
            event.value = y;
            write(fd, &event, sizeof(event));
        }
    }

    inline void click() {
        press();
        flush();
        release();
        flush();
    }

    inline void press() {
        memset(&event, 0, sizeof(event));
        event.type = EV_KEY;
        event.code = BTN_LEFT;
        event.value = 1;
        write(fd, &event, sizeof(event));
    }

    inline void release() {
        memset(&event, 0, sizeof(event));
        event.type = EV_KEY;
        event.code = BTN_LEFT;
        event.value = 0;
        write(fd, &event, sizeof(event));
    }

    inline void flush() {
        memset(&event_end, 0, sizeof(event_end));
        gettimeofday(&event_end.time, NULL);
        event_end.type = EV_SYN;
        event_end.code = SYN_REPORT;
        event_end.value = 0;
        write(fd, &event_end, sizeof(event_end));
    }

    inline void close() {
        if (fd < 0) {
            return;
        }
        ::close(fd);
        fd = -1;
    }
};