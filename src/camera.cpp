#include "camera.h"
#include "main.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 position) {
    this->position = position;
}

glm::mat4 Camera::viewmatrix() {
    return glm::mat4();
}

glm::mat4 Camera::projectionmatrix() {
    return glm::perspective(
            glm::radians(fov),
            (float)WIDTH/HEIGHT,
            0.1f,
            100.f
    );
}

glm::mat4 Camera::vpmatrix() {
    return this->projectionmatrix() * this->viewmatrix();
}

PYR_Camera::PYR_Camera(glm::vec3 position, float pitch, float yaw) {
    this->position = position;
    this->pitch = pitch;
    this->yaw = yaw;
}

glm::mat4 PYR_Camera::viewmatrix() {
    // Minecraft style: +Z is south and 0 degrees, clockwise pitch
    glm::vec3 target = forward();

    return glm::lookAt(
            position,
            position + target,
            up()
    );
}

glm::vec3 PYR_Camera::forward() {
    glm::vec3 out;

    out.x = -glm::sin(yaw);
    out.z = glm::cos(yaw);
    out.y = glm::tan(pitch);

    return glm::normalize(out);
}

glm::vec3 PYR_Camera::up() {
    return glm::rotate(glm::mat4(1.0f), roll, forward()) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}
