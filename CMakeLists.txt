cmake_minimum_required(VERSION 4.1.0)

option(BUILD_ENGINE "Build the main engine" OFF)
option(BUILD_GAME "Build the game" OFF)

if (BUILD_ENGINE)
    project(gingey
    LANGUAGES C
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/engine)

    add_subdirectory(libs/GLFW)

    add_executable(gingey engine/src/core/main.c)

    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()

    find_package(Vulkan REQUIRED COMPONENTS glslc)

    target_include_directories(gingey PRIVATE engine/include)

    target_link_libraries(gingey PRIVATE Vulkan::Vulkan glfw)

    set(ENGINE_SOURCES
        engine/src/core/engine.c
        engine/src/core/main.c
        engine/src/core/memory.c
        engine/src/core/logger.c
        engine/src/core/event_system.c
    )

    set(PLATFORM_SOURCES
        engine/src/platform/platform.c
        $<$<PLATFORM_ID:Windows>:engine/src/platform/win32_platform.c>
    )

    set(GRAPHICS_SOURCES
        engine/src/graphics/vulkan_renderer.c
        engine/src/graphics/vulkan_utils.c
        engine/src/graphics/shader.c
        engine/src/graphics/texture.c
    )

    target_sources(gingey PRIVATE
        ${ENGINE_SOURCES}
        ${PLATFORM_SOURCES}
        ${GRAPHICS_SOURCES}
    )

    if(WIN32)
        target_link_libraries(gingey PRIVATE user32 gdi32)
    endif()

    target_compile_options(gingey PRIVATE
      $<$<C_COMPILER_ID:MSVC>:/W4 /WX>
      $<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror -Wno-empty-translation-unit>
    )
endif()

if (BUILD_GAME)
    project(RogueLikeSim
    LANGUAGES C
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/game)

    add_executable(rogue_like_sim game/src/main.c)

    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()

    target_include_directories(rogue_like_sim PRIVATE include)
endif()