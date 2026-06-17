cmake_minimum_required(VERSION 3.30)
project(smoke_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pull in the graphics library sitting two levels up.
# In a real standalone project this would instead be:
#   add_subdirectory(/Users/oscar/code/c/graphics graphics-build)
# or a FetchContent_Declare pointing at the graphics git repo.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. graphics-build)

add_executable(smoke_test main.cpp)
target_link_libraries(smoke_test PRIVATE graphics::graphics)

# Copy shaders next to the binary so the cwd-relative paths in main.cpp
# resolve no matter where the executable is launched from.
add_custom_command(TARGET smoke_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders
                $<TARGET_FILE_DIR:smoke_test>/shaders)

target_compile_definitions(smoke_test PRIVATE
        SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders")