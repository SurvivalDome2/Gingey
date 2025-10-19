cmake_minimum_required(VERSION 4.1.0)

option(BUILD_ENGINE "Build the main engine" OFF)
option(BUILD_GAME "Build the game" OFF)

if (BUILD_ENGINE)
    project(ginger LANGUAGES C)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/engine)

    set(CMAKE_C_STANDARD 23)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()

    find_package(Vulkan REQUIRED COMPONENTS glslc)

    set(ENGINE_SOURCES
        engine/src/core/engine.c
        engine/src/core/memory.c
        engine/src/core/logger.c
        engine/src/core/event_system.c
        engine/src/core/gr_utils.c
        engine/src/core/multi_thread.c
        engine/src/core/window.c
    )

    set(GRAPHICS_SOURCES
        engine/src/graphics/vulkan_renderer.c
        engine/src/graphics/vulkan_utils.c
        engine/src/graphics/shader.c
        engine/src/graphics/texture.c
    )

    add_library(ginger STATIC
        ${ENGINE_SOURCES}
        ${GRAPHICS_SOURCES}
    )

    target_sources(ginger PRIVATE
        ${ENGINE_SOURCES}
        ${GRAPHICS_SOURCES}
    )

    target_include_directories(ginger PUBLIC engine/include)

    target_link_libraries(ginger PUBLIC Vulkan::Vulkan)

    target_link_libraries(ginger PRIVATE user32 gdi32)

    target_compile_options(ginger PRIVATE
      $<$<C_COMPILER_ID:MSVC>:/W4 /WX>
      $<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror -Wno-empty-translation-unit -Wno-newline-eof -Wno-deprecated-declarations -Wno-incompatible-pointer-types-discards-qualifiers>
    )
endif()

if (BUILD_GAME)
    project(game LANGUAGES C)

    set(GINGER_LIB_PATH "${CMAKE_SOURCE_DIR}/bin/engine/ginger.lib")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/game)

    add_executable(game WIN32 game/src/core/main.c)

    set(CORE_SOURCES
        game/src/core/window_procs.c
    )

    target_sources(game PRIVATE
        ${CORE_SOURCES}
    )

    set(CMAKE_C_STANDARD 23)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()

    target_include_directories(game PRIVATE game/include engine/include)

    target_link_libraries(game PRIVATE ${GINGER_LIB_PATH} Vulkan::Vulkan)
endif()