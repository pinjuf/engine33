#pragma once

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Placement {
    public:
        Placement();
        Placement(glm::vec3 position);
        Placement(glm::vec3 position, glm::mat4 orientation);

        glm::vec3 position = glm::vec3(0.0f);
        glm::mat4 orientation = glm::mat4(1.0f);

        Placement * parent = NULL;

        glm::mat4 matrix();

        glm::vec3 worldpos();

        glm::vec3 worldaxis(glm::vec3 axis);
};
