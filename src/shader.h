#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Mesh;

#include "mesh.h"

GLuint loadShader(const char * vertp, const char * fragp);

class ShaderManager {
    public:
        ShaderManager();

        GLuint load_shader(GLenum type, const char * path);

        // TODO: Perhaps hash the string, this seems ugly
        std::map<std::string, GLuint> cache;

        // TODO: Geometry/tesselation/etc. shaders?
        GLuint link_program(GLuint vert, GLuint frag);

        void update_mesh_bufs(GLuint shader, Mesh mesh);
};
