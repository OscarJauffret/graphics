//
// Smoke test for the `graphics` library.
// Spinning, color-pulsing triangle viewed through the fly camera.
// Exercises: Window (context/clear/swap/shouldClose), Shader (file load,
// compile, link, uniform setters, hot-reload on R), Camera (view matrix +
// movement primitives).
//
// Controls: WASD move, Space/Shift up-down, mouse look, R reloads shaders,
//           Esc quits.
//

#include <GL/glew.h>
#include "graphics/window.hpp"
#include "graphics/shader.hpp"
#include "graphics/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main() {
    Window window(800, 600, "graphics smoke test");

    // The library captures the cursor? No — that's the app's choice. Do it here.
    glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    std::string dir = SHADER_DIR;
    Shader shader((dir + "/basic.vert").c_str(), (dir + "/basic.frag").c_str());

    // A triangle: position (xyz) + color (rgb), interleaved.
    float vertices[] = {
        // positions          // colors
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Mouse callback routed to the camera via the window user pointer.
    glfwSetWindowUserPointer(window.getHandle(), &camera);
    glfwSetCursorPosCallback(window.getHandle(),
        [](GLFWwindow* w, double x, double y) {
            auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
            cam->processMouse(x, y);
        });

    float lastFrame = 0.0f;
    bool  rHeldLastFrame = false;

    while (!window.shouldClose()) {
        float now = static_cast<float>(glfwGetTime());
        float dt  = now - lastFrame;
        lastFrame = now;

        GLFWwindow* h = window.getHandle();
        if (glfwGetKey(h, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(dt);
        if (glfwGetKey(h, GLFW_KEY_S) == GLFW_PRESS) camera.moveBackward(dt);
        if (glfwGetKey(h, GLFW_KEY_A) == GLFW_PRESS) camera.moveLeft(dt);
        if (glfwGetKey(h, GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(dt);
        if (glfwGetKey(h, GLFW_KEY_SPACE) == GLFW_PRESS) camera.moveUp(dt);
        if (glfwGetKey(h, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.moveDown(dt);

        // Hot-reload on a rising edge of R, so it fires once per press.
        bool rHeld = glfwGetKey(h, GLFW_KEY_R) == GLFW_PRESS;
        if (rHeld && !rHeldLastFrame) shader.reload();
        rHeldLastFrame = rHeld;

        window.clear(0.1f, 0.1f, 0.12f, 1.0f);  // clears color + depth by default

        shader.use();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), now, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj  = glm::perspective(glm::radians(45.0f),
                                           window.aspect(), 0.1f, 100.0f);
        shader.setMat4("uModel", model);
        shader.setMat4("uView", camera.getViewMatrix());
        shader.setMat4("uProj", proj);
        shader.setFloat("uTint", 0.6f + 0.4f * std::sin(now * 2.0f));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapBuffers();
        window.pollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return 0;
}
