#include "keyboard.h"

void handle_keyboard() {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_W)) cam.pitch += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_S)) cam.pitch -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_D)) cam.yaw += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_A)) cam.yaw -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_Z)) cam.roll -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_C)) cam.roll += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_E)) cam.position += cam.forward() * 0.01f;
    if (glfwGetKey(window, GLFW_KEY_Q)) cam.position -= cam.forward() * 0.01f;
}
