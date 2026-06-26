//
// Originally created by Oscar Jauffret on 14/04/2026.
// Extracted into the reusable `graphics` library; ocean-tuned defaults
// (pitch = -30, position over water) lifted into constructor parameters.
//

#ifndef GRAPHICS_CAMERA_HPP
#define GRAPHICS_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class flyByCamera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed       = 10.0f;
    float sensitivity = 0.1f;

    bool   firstMouse = true;
    double lastX = 0.0;
    double lastY = 0.0;

    explicit flyByCamera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
                    float speed_ = 10.0f,
                    float yaw_   = -90.0f,
                    float pitch_ =  0.0f
                    )
        : position(pos), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
          yaw(yaw_), pitch(pitch_), speed(speed_) {
        updateVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    // --- Movement primitives: caller decides which keys map to these,
    // --- so the camera stays free of any windowing dependency. ---
    void moveForward(float dt) {  // horizontal-plane movement
        glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        position += flatFront * (speed * dt);
    }
    void moveBackward(float dt) {
        glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        position -= flatFront * (speed * dt);
    }
    void moveRight(float dt) { position += right * (speed * dt); }
    void moveLeft (float dt) { position -= right * (speed * dt); }
    void moveUp   (float dt) { position.y += speed * dt; }
    void moveDown (float dt) { position.y -= speed * dt; }

    void processMouse(double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX) * sensitivity;
        float yoffset = static_cast<float>(lastY - ypos) * sensitivity; // inverted Y
        lastX = xpos;
        lastY = ypos;

        yaw   += xoffset;
        pitch += yoffset;

        if (pitch >  89.0f) pitch =  89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateVectors();
    }

private:
    void updateVectors() {
        glm::vec3 f;
        f.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        f.y = std::sin(glm::radians(pitch));
        f.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
};

#endif //GRAPHICS_CAMERA_HPP
