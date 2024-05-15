#include "mouse.h"
#include "main.h"

double mousexpos, mouseypos;

double plane_speed = 0;

void handle_mouse() {
    double newxpos, newypos;
    double deltaxpos, deltaypos;
    glfwGetCursorPos(window, &newxpos, &newypos);

    deltaxpos = newxpos - mousexpos;
    deltaypos = newypos - mouseypos;

    mousexpos = newxpos;
    mouseypos = newypos;

    Mesh * plane = autoloader.objects["plane"];

    plane->p.orientation = glm::rotate(glm::mat4(1.0f), float(1 * deltaT * deltaxpos), glm::vec3(plane->p.orientation * glm::vec4(FORWARD, 0.0f))) * plane->p.orientation;
    plane->p.orientation = glm::rotate(glm::mat4(1.0f), float(1 * deltaT * deltaypos), glm::vec3(plane->p.orientation * glm::vec4(RIGHT, 0.0f))) * plane->p.orientation;

    plane->p.position += glm::vec3(plane->p.orientation * glm::vec4(FORWARD, 0.0f)) * (float)deltaT * (float)plane_speed;
}
