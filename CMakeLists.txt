cmake_minimum_required(VERSION 3.25)

project(AsiPlugin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(PSDK_EXAMPLES OFF)
set(PSDK_INSTALL OFF)
set(PSDK_GTASA ON)

add_subdirectory(third-party/ktsignal)
add_subdirectory(third-party/kthook)
add_subdirectory(third-party/RakHook)
add_subdirectory(third-party/plugin-sdk)
add_subdirectory(third-party/SAMP-API)
add_subdirectory(third-party/imgui)

file(GLOB_RECURSE PROJECT_HEADERS src/*.h)
file(GLOB_RECURSE PROJECT_SOURCE_HEADERS src/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCE_HEADERS}
    ${PROJECT_SOURCES}
)

if (NOT "${DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Lib/x86)
elseif (NOT "$ENV{DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Lib/x86)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PUBLIC
    d3d9
    d3dx9
    kthook
    rakhook
    pluginsdk::gtasa
    sampapi
    imgui
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".asi"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)