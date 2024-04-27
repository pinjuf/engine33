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

#define WIDTH 1920
#define HEIGHT 1080
