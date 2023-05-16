#include "configuration.h"
#include "sdk/logging.h"

#include <math.h>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>
#include <pwd.h>

void configuration::reload(std::string path) {
    std::ifstream file(path);
    try {
        settings = json::parse(file);

        try_get<float>("aim.strength", aim_strength);
        try_get<float>("aim.deadzone.x", aim_deadzone.x);
        try_get<float>("aim.deadzone.y", aim_deadzone.y);
        try_get<float>("aim.fov", aim_fov);
        try_get<int>("aim.hitbox", aim_hitbox);

        try_get<float>("esp.friendly_color.r", esp_friendly_color.x);
        try_get<float>("esp.friendly_color.g", esp_friendly_color.y);
        try_get<float>("esp.friendly_color.b", esp_friendly_color.z);
        try_get<float>("esp.enemy_color.r", esp_enemy_color.x);
        try_get<float>("esp.enemy_color.g", esp_enemy_color.y);
        try_get<float>("esp.enemy_color.b", esp_enemy_color.z);
        try_get<float>("esp.thickness", esp_thickness);
        try_get<bool>("esp.outline", esp_outline);

        try_get<float>("glow.friendly_color.r", glow_friendly_color.x);
        try_get<float>("glow.friendly_color.g", glow_friendly_color.y);
        try_get<float>("glow.friendly_color.b", glow_friendly_color.z);
        try_get<float>("glow.enemy_color.r", glow_enemy_color.x);
        try_get<float>("glow.enemy_color.g", glow_enemy_color.y);
        try_get<float>("glow.enemy_color.b", glow_enemy_color.z);
        try_get<bool>("glow.visible_only", glow_visible_only);

        try_get<float>("recoil_control.strength.x", recoil_control_strength.x);
        try_get<float>("recoil_control.strength.y", recoil_control_strength.y);

        logger::info("Reloaded config file at %s", path.c_str());
    } catch (json::parse_error &e) {
        logger::error("Failed to parse the config file");
    }
    file.close();
}

void configuration::init() {
    std::string _cfg = std::string(getpwuid(getuid())->pw_dir) + "/ayypex_settings.json";
    const char* config_path = _cfg.c_str();

    logger::info("Loading configuration from %s", config_path);

    // Start watching for changes
    std::thread inotify_thread([config_path, this]() {
        int fd = inotify_init();
        if (fd < 0) {
            logger::error("Failed to initialize inotify");
            return;
        }
        logger::debug("Initialized inotify");

        int wd = inotify_add_watch(fd, config_path, IN_MODIFY);
        if (wd < 0) {
            logger::error("Failed to add inotify watch, reason: %s", strerror(errno));
            return;
        }

        reload(config_path);

        logger::debug("Added inotify watch");
        logger::info("Started watching for configuration changes");

        char buffer[1024];
        while (true) {
            int length = read(fd, buffer, 1024);
            if (length < 0) {
                logger::error("Failed to read inotify event");
                return;
            }

            int i = 0;
            while (i < length - 1) {
                inotify_event *event = (inotify_event *)&buffer[i];
                if (event->mask & IN_CLOSE_WRITE) {
                    logger::debug("Reloading configuration");
                    this->reload(config_path);
                }
                i += sizeof(inotify_event) + event->len;
            }
        }
    });
    inotify_thread.detach();
}

configuration settings;