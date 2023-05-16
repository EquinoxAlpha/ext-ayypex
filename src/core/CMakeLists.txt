cmake_minimum_required(VERSION 3.26)

target_sources(ayypex PRIVATE
    common.h
    configuration.h
    configuration.cpp
    core.h
    core.cpp
    module.h
    mouse.h
    offsets.h
)
add_subdirectory(modules)
add_subdirectory(sdk)