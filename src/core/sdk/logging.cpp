#include "logging.hpp"

#include <cstdarg>
#include <iostream>
#include <cstdio>

namespace logger {
    void info(const char* fmt, ...) {
        std::cout << "\033[1;32m[INFO] ";
        std::cout << "\033[0m";
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        std::cout << std::endl;
    }

    void debug(const char* fmt, ...) {
        if (!debugging) return;
        std::cout << "\033[1;34m[DEBUG] ";
        std::cout << "\033[0m";
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        std::cout << std::endl;
    }

    void warn(const char* fmt, ...) {
        std::cout << "\033[1;33m[WARN] ";
        std::cout << "\033[0m";
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        std::cout << std::endl;
    }

    void error(const char* fmt, ...) {
        std::cout << "\033[1;31m[ERROR] ";
        std::cout << "\033[0m";
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        std::cout << std::endl;
    }

    bool debugging = true;
}