cmake_minimum_required(VERSION 3.15)

project(imgui)

add_library(${PROJECT_NAME} STATIC
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
    src/imgui_demo.cpp
    src/imgui_impl_win32.cpp
    src/imgui_impl_dx9.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)