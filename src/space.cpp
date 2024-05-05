#include "space.h"

Placement::Placement() {}

Placement::Placement(glm::vec3 position) {
    this->position = position;
}

Placement::Placement(glm::vec3 position, glm::mat4 orientation) {
    this->position = position;
    this->orientation = orientation;
}

glm::mat4 Placement::matrix() {
    // This placements's transformation matrix (includes parents)
    if (parent) {
        return parent->matrix() * glm::translate(glm::mat4(1.0f), position) * orientation;
    }

    return glm::translate(glm::mat4(1.0f), position) * orientation;
}

glm::vec3 Placement::worldpos() {
    // Position of the object in world space
    if (parent) {
        return glm::vec3(parent->matrix() * glm::vec4(position, 1.0f));
    }

    return position;
}

glm::vec3 Placement::worldaxis(glm::vec3 axis) {
    // An axis relative to this placements rotation (and its parents)
    return glm::vec3(matrix() * glm::vec4(axis, 0.0f));
}
