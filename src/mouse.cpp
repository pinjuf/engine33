#include "mouse.h"
#include "main.h"

double mousexpos, mouseypos;

double plane_speed = 20;

void handle_mouse() {
    double newxpos, newypos;
    double deltaxpos, deltaypos;
    glfwGetCursorPos(window, &newxpos, &newypos);

    deltaxpos = newxpos - mousexpos;
    deltaypos = newypos - mouseypos;

    mousexpos = newxpos;
    mouseypos = newypos;

    plane.orientation = glm::rotate(glm::mat4(1.0f), float(1 * deltaT * deltaxpos), glm::vec3(plane.orientation * glm::vec4(FORWARD, 0.0f))) * plane.orientation;
    plane.orientation = glm::rotate(glm::mat4(1.0f), float(1 * deltaT * deltaypos), glm::vec3(plane.orientation * glm::vec4(RIGHT, 0.0f))) * plane.orientation;

    plane.position += glm::vec3(plane.orientation * glm::vec4(FORWARD, 0.0f)) * (float)deltaT * (float)plane_speed;

    cam.position = plane.position \
                   + glm::vec3(plane.orientation * glm::vec4(BACKWARD, 0.0f)) * (float)3 \
                   + glm::vec3(plane.orientation * glm::vec4(UP, 0.0f)) * (float)0.5;
    cam.orientation = plane.orientation;
}
