#pragma once

#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "autoload.h"

extern GLFWwindow * window;

extern Camera cam;
extern AutoLoader autoloader;
extern ShaderManager shadermanager;

extern double seconds; // Time since the program has been running
extern double deltaT;

extern int width, height;

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
