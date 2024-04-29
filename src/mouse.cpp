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

    cam.chyaw_g(deltaxpos * (0.5f * (float)deltaT));
    cam.chpitch(- deltaypos * (0.5f * (float)deltaT));
}
