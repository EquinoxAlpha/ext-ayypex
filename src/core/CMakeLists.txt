cmake_minimum_required(VERSION 3.26)

target_sources(ayypex PRIVATE
    configuration.cpp
    core.cpp
)
add_subdirectory(modules)
add_subdirectory(sdk)