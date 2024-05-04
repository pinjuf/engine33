#include "mesh.h"
#include "shader.h"
#include "main.h"

#include <cstring>

Mesh::Mesh() {
    init_glbufs();
}

void Mesh::load_wfobj(const char * path) {
    // "Inspired" (copied) from https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

    // TODO: Indexing!

    FILE * file = fopen(path, "r");
    if (!file) {
        std::cerr << "Could not open model file " << path << std::endl;
        return;
    }

    vertices.clear();
    uvs.clear();
    normals.clear();
    indices.clear();

    std::vector<glm::vec3> v{{0.0f, 0.0f, 0.0f}};
    std::vector<glm::vec2> vt{{0.0f, 0.0f}};
    std::vector<glm::vec3> vn{{0.0f, 0.0f, 0.0f}};

    std::vector<std::string> f{};

    while (true) {
        char token[128];
        // TODO: Buffer overflow possible here!
        if (fscanf(file, "%s ", token) == EOF)
            break;

        if (!strcmp(token, "v")) { // Coords
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            v.push_back(vertex);
        } else if (!strcmp(token, "vt")) { // UVs
            glm::vec2 texcoord;
            fscanf(file, "%f %f\n", &texcoord.x, &texcoord.y);
            texcoord.y *= -1;
            vt.push_back(texcoord);
        } else if (!strcmp(token, "vn")) { // Normals
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            vn.push_back(normal);
        } else if (!strcmp(token, "f")) { // Indices
            // Note how we assume we already have the points referred to by the indices in our temporary v, vt & vn

            for (uint8_t i = 0; i < 3; i++) {
                char index[128];
                fscanf(file, "%s", index);
                f.push_back(std::string(index));
            }
        }
    }

    // If an index group ("ddd/ddd/ddd") is already known (all 3 indices match exactly, we can reuse the index)
    std::map<std::string, GLuint> known_indices;
    for (std::string index_group : f) {

        auto known = known_indices.find(index_group);
        if (known != known_indices.end()) {
            indices.push_back(known->second);
            continue;
        }

        int vert_index, uv_index, norm_index;
        sscanf(index_group.c_str(), "%d/%d/%d", &vert_index, &uv_index, &norm_index);

        glm::vec3 V = v[vert_index];
        glm::vec2 VT = vt[uv_index];
        glm::vec3 VN = vn[norm_index];

        indices.push_back(vertices.size()/3); // We do this BEFORE pushing our values!
        known_indices[index_group] = indices.back();

        vertices.push_back(V.x);
        vertices.push_back(V.y);
        vertices.push_back(V.z);

        uvs.push_back(VT.x);
        uvs.push_back(VT.y);

        normals.push_back(VN.x);
        normals.push_back(VN.y);
        normals.push_back(VN.z);
    }
}

void Mesh::init_glbufs() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(5, (GLuint*)&glbufs);
}

glm::mat4 Mesh::model_matrix() {
    return glm::translate(glm::mat4(1.0f), worldspace_pos) * orientation;
}

void Mesh::update_mesh_bufs(ShaderProgram shader) {
    // Update the vertex attributes (the connection between the mesh's buffer and the shaders)

    glBindVertexArray(vao);

    // TODO: this is ugly
    if (shader.vertex_attributes.position != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, glbufs.vertices_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            shader.vertex_attributes.position,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (shader.vertex_attributes.uv != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, glbufs.uvs_buffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), &uvs[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            shader.vertex_attributes.uv,
            2,        // vec2
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (shader.vertex_attributes.normal != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, glbufs.normals_buffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            shader.vertex_attributes.normal,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (shader.vertex_attributes.color != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, glbufs.colors_buffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            shader.vertex_attributes.color,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (indices.size()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glbufs.indices_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::render(ShaderProgram shader) {
    // Guess what this does!

    glm::mat4 m = model_matrix();
    glm::mat4 v = cam.viewmatrix();
    glm::mat4 p = cam.projectionmatrix();
    glm::mat4 mvp = p * v * m;

    glBindVertexArray(vao);
    glUseProgram(shader.id);

    if (shader.uniforms.modelmatrix != -1)
        glUniformMatrix4fv(shader.uniforms.modelmatrix, 1, GL_FALSE, &m[0][0]);
    if (shader.uniforms.cameramatrix != -1)
        glUniformMatrix4fv(shader.uniforms.cameramatrix, 1, GL_FALSE, &v[0][0]);
    if (shader.uniforms.projectionmatrix != -1)
        glUniformMatrix4fv(shader.uniforms.projectionmatrix, 1, GL_FALSE, &p[0][0]);
    if (shader.uniforms.mvp != -1)
        glUniformMatrix4fv(shader.uniforms.mvp, 1, GL_FALSE, &mvp[0][0]);

    if (shader.vertex_attributes.position != -1)
        glEnableVertexAttribArray(shader.vertex_attributes.position);
    if (shader.vertex_attributes.uv != -1)
        glEnableVertexAttribArray(shader.vertex_attributes.uv);
    if (shader.vertex_attributes.normal != -1)
        glEnableVertexAttribArray(shader.vertex_attributes.normal);
    if (shader.vertex_attributes.color != -1)
        glEnableVertexAttribArray(shader.vertex_attributes.color);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glbufs.indices_buffer);
    glDrawElements(
        GL_TRIANGLES,
        indices.size(),
        GL_UNSIGNED_INT,
        (void*)0
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (shader.vertex_attributes.position != -1)
        glDisableVertexAttribArray(shader.vertex_attributes.position);
    if (shader.vertex_attributes.uv != -1)
        glDisableVertexAttribArray(shader.vertex_attributes.uv);
    if (shader.vertex_attributes.normal != -1)
        glDisableVertexAttribArray(shader.vertex_attributes.normal);
    if (shader.vertex_attributes.color != -1)
        glDisableVertexAttribArray(shader.vertex_attributes.color);

    glBindVertexArray(0);
}
