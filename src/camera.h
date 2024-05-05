#pragma once

#include <glm/glm.hpp> // 3D maths
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "space.h"

// There are multiple Camera classes.
// They mainly differ in the way the treat rotation.
class Camera {
    public:
        Camera();

        Placement p;

        float fov = 100; // degrees

        glm::mat4 viewmatrix();
        glm::mat4 projectionmatrix();
        glm::mat4 vpmatrix();
};
