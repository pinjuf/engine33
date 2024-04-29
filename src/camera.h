#pragma once

#include <glm/glm.hpp> // 3D maths
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

// There are multiple Camera classes.
// They mainly differ in the way the treat rotation.
class Camera {
    public:
        Camera();
        Camera(glm::vec3 position);

        glm::vec3 position = glm::vec3(0, 0, 0);

        float fov = 100; // degrees

        glm::mat4 orientation = glm::mat4(1.0f);

        glm::mat4 viewmatrix();
        glm::mat4 projectionmatrix();
        glm::mat4 vpmatrix();

        glm::vec3 forward();
        glm::vec3 right();
        glm::vec3 up();

        void chyaw_g(float delta); // _g because this rotates around the GLOBAL up axis
        void chpitch(float delta);
};
