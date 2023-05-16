#include "core/core.h"

std::chrono::high_resolution_clock timer;
std::chrono::time_point<std::chrono::high_resolution_clock> last_tick;

int main(int argc, char** argv) {
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
    return 0;
}