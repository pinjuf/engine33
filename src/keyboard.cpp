#include "keyboard.h"
#include "main.h"

#include "mouse.h"

void handle_keyboard() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        plane_speed += 20 * deltaT;
        cam.fov += deltaT * 5;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        plane_speed -= 20 * deltaT;
        cam.fov -= deltaT * 5;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        plane.orientation = glm::rotate(glm::mat4(1.0f), float(1 * deltaT), glm::vec3(plane.orientation * glm::vec4(UP, 0.0f))) * plane.orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        plane.orientation = glm::rotate(glm::mat4(1.0f), float(-1 * deltaT), glm::vec3(plane.orientation * glm::vec4(UP, 0.0f))) * plane.orientation;
}
