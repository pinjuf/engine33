#include "camera.h"
#include "main.h"

Camera::Camera() {}

glm::mat4 Camera::viewmatrix() {
    return glm::lookAt(
            p.worldpos(),
            p.worldpos() + p.worldaxis(FORWARD),
            glm::vec3(p.worldaxis(UP))
    );
}

glm::mat4 Camera::projectionmatrix() {
    return glm::perspective(
            glm::radians(fov),
            (float)WIDTH/HEIGHT,
            0.01f, // Near limit
            1000.f  // Far limit
    );
}

glm::mat4 Camera::vpmatrix() {
    return this->projectionmatrix() * this->viewmatrix();
}
