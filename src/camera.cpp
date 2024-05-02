#include "camera.h"
#include "main.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 position) {
    this->position = position;
}

glm::mat4 Camera::viewmatrix() {
    return glm::lookAt(
            position,
            position + glm::vec3(orientation * glm::vec4(FORWARD, 1.0f)),
            glm::vec3(orientation * glm::vec4(UP, 1.0f))
    );
}

glm::mat4 Camera::projectionmatrix() {
    return glm::perspective(
            glm::radians(fov),
            (float)WIDTH/HEIGHT,
            0.01f,
            100.f
    );
}

glm::mat4 Camera::vpmatrix() {
    return this->projectionmatrix() * this->viewmatrix();
}

glm::vec3 Camera::forward() {
    return orientation * glm::vec4(FORWARD, 0.0f);
}

glm::vec3 Camera::right() {
    return orientation * glm::vec4(RIGHT, 0.0f);
}

glm::vec3 Camera::up() {
    return orientation * glm::vec4(UP, 0.0f);
}

void Camera::chyaw_g(float delta) {
    // We have Minecraft-like yaw
    orientation = glm::rotate(glm::mat4(1.0f), -delta, UP) * orientation;
}

void Camera::chpitch(float delta) {
    glm::mat4 new_orientation = glm::rotate(glm::mat4(1.0f), delta, right()) * orientation;

    if ((new_orientation * glm::vec4(UP, 0.0f)).y < 0)
        return;

    orientation = new_orientation;
}
