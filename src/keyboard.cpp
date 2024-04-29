#include "keyboard.h"
#include "main.h"

void handle_keyboard() {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_W)) cam.position += cam.forward() * (2.0f * (float)deltaT);
    if (glfwGetKey(window, GLFW_KEY_S)) cam.position -= cam.forward() * (2.0f * (float)deltaT);
    if (glfwGetKey(window, GLFW_KEY_A)) cam.position -= cam.right() * (2.0f * (float)deltaT);
    if (glfwGetKey(window, GLFW_KEY_D)) cam.position += cam.right() * (2.0f * (float)deltaT);
}