cmake_minimum_required(VERSION 3.26)
project(ayypex)

set(EXTERNAL_SRC
        external/imgui/imgui.h
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_impl_glfw.h
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_opengl3.h
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_impl_opengl3_loader.h
        external/glad.c
        )

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_executable(ayypex ${EXTERNAL_SRC})

target_include_directories(ayypex PUBLIC external)
target_compile_features(ayypex PUBLIC cxx_std_20)
target_link_libraries(ayypex ${CMAKE_SOURCE_DIR}/external/GLFW/libglfw3.a)
target_link_libraries(ayypex GL X11 pthread Xrandr Xi dl)
target_include_directories(ayypex PUBLIC external)

add_subdirectory(src)