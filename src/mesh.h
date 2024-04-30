#pragma once

#include "shader.h"

class Mesh {
    public:
        Mesh();

        std::vector<GLfloat> vertices; // x, y, z, x, y, z, ...
        std::vector<GLfloat> uvs; // u, v, u, v, ...
        std::vector<GLfloat> normals; // x, y, z, x, y, z, ...
        std::vector<GLuint> indices;

        void load_wfobj(const char * path);
};
