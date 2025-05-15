cmake_minimum_required (VERSION 3.8)

project(renderer)

add_library (${PROJECT_NAME} "renderer.cpp")
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

file(GLOB DX9_SEARCH_PATHS
    "C:/Program Files (x86)/Microsoft DirectX SDK*/Include"
    "$ENV{DXSDK_DIR}/Include"
    "$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Include"
)

find_path(DX9_INCLUDE_DIRS
    NAMES d3dx9.h 
    PATHS ${DX9_SEARCH_PATHS}
)

find_library(DX9_LIB d3dx9.lib "${DX9_INCLUDE_DIRS}/../Lib/x86/" NO_DEFAULT_PATH)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${DX9_LIB}
    INTERFACE renderer
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${DX9_INCLUDE_DIRS}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)