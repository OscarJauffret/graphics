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
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float distance;
    float phi;
    float theta;

    FixedCamera(float distance_, glm::vec3 target_, float theta_ = std::numbers::pi/2, float phi_ = std::numbers::pi/3):
    distance(distance_), phi(phi_), theta(theta_), target(target_) {
        computeCameraPosition();
    }

    glm::mat4 getViewMatrix(glm::vec3 target) {
        return glm::lookAt(computeCameraPosition(), target, worldUp);
    }

private:
    glm::vec3 computeCameraPosition() {
        position = target;
        position.x += distance * cos(theta) * sin(phi);
        position.y += distance * sin(theta) * sin(phi);
        position.x += distance * cos(phi);
    }

    glm::vec3 getCameraDirection() {
        return glm::normalize(position - target);
    }

    glm::vec3 getCameraRight() {
        return glm::normalize(glm::cross(worldUp, getCameraDirection()));
    }

    glm::vec3 getCameraUp() {
        return glm::cross(getCameraDirection(), getCameraRight());
    }
};

#endif //GRAPHICS_FIXEDCAMERA_HPP
