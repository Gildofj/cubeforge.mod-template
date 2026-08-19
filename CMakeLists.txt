cmake_minimum_required(VERSION 3.20)
project(CubeForgeSampleMod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS DIRECTINPUT_VERSION=0x0800)
    add_compile_options(/W3 /EHsc /utf-8 /wd4828)
endif()

# Fetch and configure CubeForge SDK
include(FetchContent)
if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

FetchContent_Declare(
    cubeforge_sdk
    GIT_REPOSITORY https://github.com/Gildofj/cubeforge.sdk.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(cubeforge_sdk)

# Mod DLL Target
add_library(CubeForgeSampleMod SHARED
    src/main.cpp
    src/SampleMod.cpp
    src/SampleMod.h
)

target_include_directories(CubeForgeSampleMod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${cubeforge_sdk_SOURCE_DIR}
)

target_link_libraries(CubeForgeSampleMod PRIVATE CWSDK)

if(MSVC)
    set_property(TARGET CubeForgeSampleMod PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
