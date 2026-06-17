//
// Originally created by Oscar Jauffret on 11/04/2026.
// Extracted into the reusable `graphics` library.
//

#ifndef GRAPHICS_WINDOW_HPP
#define GRAPHICS_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose();
    void swapBuffers();
    void pollEvents();

    // Clears color + depth by default. Pass a custom mask if you want
    // color only (e.g. GL_COLOR_BUFFER_BIT for a 2D sim).
    void clear(float r, float g, float b, float a,
               GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLFWwindow* getHandle() { return m_window; }

    int width()  const { return m_width;  }
    int height() const { return m_height; }
    float aspect() const {
        return m_height == 0 ? 1.0f
                             : static_cast<float>(m_width) / static_cast<float>(m_height);
    }

private:
    GLFWwindow* m_window;
    int m_width, m_height;
};

#endif //GRAPHICS_WINDOW_HPP
