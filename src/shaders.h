#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

GLuint loadShader(const char * vertp, const char * fragp);
