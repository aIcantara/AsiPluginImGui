cmake_minimum_required(VERSION 3.15)

project(RakHook VERSION 1.0 LANGUAGES CXX C)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(RAKHOOK_MASTER_PROJECT ON)
endif()

add_library(rakhook STATIC)
add_library(rakhook::rakhook ALIAS rakhook)

target_include_directories(rakhook PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(rakhook PRIVATE
    src/RakHook/rakhook.cpp

    src/RakNet/BitStream.cpp
    src/RakNet/DS_HuffmanEncodingTree.cpp
    src/RakNet/PluginInterface.cpp
    src/RakNet/StringCompressor.cpp
)

target_link_libraries(rakhook PRIVATE kthook)

target_compile_features(rakhook PUBLIC cxx_std_20)

set_property(TARGET rakhook PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")