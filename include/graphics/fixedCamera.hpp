//
// Created by Oscar Jauffret on 26/06/2026.
//

#ifndef GRAPHICS_FIXEDCAMERA_HPP
#define GRAPHICS_FIXEDCAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <numbers>

class FixedCamera {
public:
    glm::vec3 target;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float distance;
    float phi;
    float theta;

    // Mouse
    float sensitivity = 0.1f;
    bool firstMouse = true;
    double lastX = 0.0;
    double lastY = 0.0;

    FixedCamera(float distance_, glm::vec3 target_, float theta_ = std::numbers::pi/2, float phi_ = std::numbers::pi/3):
    distance(distance_), phi(phi_), theta(theta_), target(target_) {}

    glm::mat4 getViewMatrix(glm::vec3 target_) {
        target = target_;
        return glm::lookAt(getCameraPosition(), target_, worldUp);
    }


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

        phi += xoffset;
        theta += yoffset;

        if (theta > std::numbers::pi/2) theta = std::numbers::pi/2;
        if (theta < -std::numbers::pi/2) theta = -std::numbers::pi/2;
        phi %= std::numbers::pi * 2;

    }

private:
    glm::vec3 getCameraPosition() {
        glm::vec3 position = target;
        position.x += distance * cos(theta) * sin(phi);
        position.y += distance * sin(theta) * sin(phi);
        position.x += distance * cos(phi);

        return position;
    }

    glm::vec3 getCameraDirection() {
        return glm::normalize(getCameraPosition() - target);
    }

    glm::vec3 getCameraRight() {
        return glm::normalize(glm::cross(worldUp, getCameraDirection()));
    }

    glm::vec3 getCameraUp() {
        return glm::cross(getCameraDirection(), getCameraRight());
    }
};

#endif //GRAPHICS_FIXEDCAMERA_HPP
