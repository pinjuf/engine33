#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.h"

extern GLFWwindow * window;

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
   -1.0f, -1.0f, 1.0f,
   1.0f, -1.0f, 1.0f,
   0.0f,  1.0f, 1.0f,
};

extern PYR_Camera cam;

extern double seconds; // Time since the program has been running
extern double deltaT;

#define WIDTH 1920
#define HEIGHT 1080

#define FORWARD (glm::vec3(0.0f, 0.0f, 1.0f))
#define BACKWARD (glm::vec3(0.0f, 0.0f, -1.0f))
#define UP (glm::vec3(0.0f, 1.0f, 0.0f))
#define DOWN (glm::vec3(0.0f, -1.0f, 0.0f))
#define LEFT (glm::vec3(1.0f, 0.0f, 0.0f))
#define RIGHT (glm::vec3(-1.0f, 0.0f, 0.0f))

#define SOUTH FORWARD
#define NORTH FORWARD
#define EAST LEFT
#define WEST RIGHT
