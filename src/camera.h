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

        virtual glm::mat4 viewmatrix();
        virtual glm::mat4 projectionmatrix();
        glm::mat4 vpmatrix();
};

// Pitch-yaw camera
class PYR_Camera : public Camera {
    public:
        using Camera::Camera;
        PYR_Camera(glm::vec3 position, float pitch, float yaw);
        float pitch = 0;
        float yaw = 0;
        float roll = 0;

        glm::mat4 viewmatrix() override;

        glm::vec3 forward();
        glm::vec3 up();

    private:
        void clamp();
};
