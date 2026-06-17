cmake_minimum_required(VERSION 3.30)
project(graphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On macOS, look in Homebrew's directory for libraries
if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif ()

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
# GLM is header-only; its bundled config can be too old for modern CMake,
# so find it quietly and fall back to a plain interface target.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
find_package(glm QUIET)
if (NOT TARGET glm::glm)
    add_library(glm::glm INTERFACE IMPORTED)
endif ()

add_library(graphics STATIC
        src/window.cpp
        src/shader.cpp
)

# PUBLIC include dir: consumers that link `graphics` get the headers,
# so they can `#include "graphics/window.hpp"`.
target_include_directories(graphics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (APPLE)
    target_include_directories(graphics PUBLIC "/opt/homebrew/include")
endif ()

# PUBLIC linkage: these propagate to anything that links `graphics`,
# so a consuming project doesn't have to re-find GLFW/GLEW/etc.
target_link_libraries(graphics PUBLIC
        glfw
        GLEW::GLEW
        glm::glm
        OpenGL::GL
)

if (APPLE)
    target_link_libraries(graphics PUBLIC
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif ()

# Optional alias so consumers can write `graphics::graphics` (matches the
# convention of GLEW::GLEW etc.) regardless of how they pull this in.
add_library(graphics::graphics ALIAS graphics)
