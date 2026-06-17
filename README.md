# graphics

A small, reusable OpenGL scaffolding library extracted from OceanSim.
Domain-agnostic infrastructure only: windowing, shader loading, and a
fly-camera. No simulation-specific code.

## Contents

- `Window` — GLFW window + GL 4.1 core context + GLEW init.
- `Shader` — file-based GLSL program with uniform setters and hot-reload.
- `Camera` — header-only fly camera (windowing-agnostic movement primitives).

## Requirements

GLFW3, GLEW, GLM, OpenGL. On macOS these come from Homebrew
(`brew install glfw glew glm`).

## Usage in a project

In the consuming project's `CMakeLists.txt`:

```cmake
add_subdirectory(/path/to/graphics graphics-build)   # or FetchContent
target_link_libraries(MySim PRIVATE graphics::graphics)
```

Then:

```cpp
#include "graphics/window.hpp"
#include "graphics/shader.hpp"
#include "graphics/camera.hpp"
```

## Notes / known limitations

- GLFW init and `glfwTerminate()` are process-global, so creating a second
  `Window` and destroying the first will tear down GLFW for both. Single
  window only, for now.
- `Camera::processMouse` reads raw cursor positions; the caller owns key
  polling and calls the `move*` primitives.
- Shader paths are resolved relative to the working directory. Either run
  from a known cwd or copy shaders next to the binary in your build.
# graphics
