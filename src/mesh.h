#pragma once

class ShaderProgram;

#include "shader.h"
#include "space.h"

#include <glm/gtc/matrix_transform.hpp>

class Mesh {
    public:
        Mesh();

        std::vector<GLfloat> vertices; // x, y, z, x, y, z, ...
        std::vector<GLfloat> uvs; // u, v, u, v, ...
        std::vector<GLfloat> normals; // x, y, z, x, y, z, ...
        std::vector<GLfloat> colors; // r, g, b, r, g, b, ...
        std::vector<GLuint> indices;

        void load_wfobj(const char * path);

        Placement p;

        glm::mat4 model_matrix();

        // GL buffers
        GLuint vao = UINT32_MAX;
        struct {
            GLuint vertices_buffer = UINT32_MAX;
            GLuint uvs_buffer      = UINT32_MAX;
            GLuint normals_buffer  = UINT32_MAX;
            GLuint colors_buffer   = UINT32_MAX;
            GLuint indices_buffer  = UINT32_MAX;
        } glbufs;

        GLuint textures[16] = {UINT32_MAX};

        void init_glbufs();
        void update_mesh_bufs(ShaderProgram shader);

        void render(ShaderProgram shader);
};
