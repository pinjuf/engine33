#include "mouse.h"
#include "main.h"

#include <iostream>

double mousexpos, mouseypos;

void handle_mouse() {
    double newxpos, newypos;
    double deltaxpos, deltaypos;
    glfwGetCursorPos(window, &newxpos, &newypos);

    deltaxpos = newxpos - mousexpos;
    deltaypos = newypos - mouseypos;

    mousexpos = newxpos;
    mouseypos = newypos;

    cam.yaw += deltaxpos * (0.5f * (float)deltaT);
    cam.pitch -= deltaypos * (0.5f * (float)deltaT);
}
