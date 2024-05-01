#include "shader.h"

ShaderManager::ShaderManager() {}

GLuint ShaderManager::load_shader(GLenum type, const char * path) {
    // Check if we already cached this shader and therefore have its "name" (id)
    auto cached = cache.find(path);
    if (cached != cache.end()) {
        return cached->second;
    }

    GLuint id = glCreateShader(type);
    std::string code;
    std::stringstream tmp;

    GLint result = GL_FALSE;
    int info_log_len;

    // Read the code into a string
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Couldn't open shader " << path << std::endl;
        return 0;
    }
    tmp << file.rdbuf();
    code = tmp.str();

    // Compile it!
    const char * code_ptr = code.c_str();
    glShaderSource(id, 1, &code_ptr, NULL);
    glCompileShader(id);

    // Check for warnings
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
        std::cout << "Error compiling " << path << std::endl;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetShaderInfoLog(id, info_log_len, NULL, log);
        std::cout << log;

        return 0;
	}

    cache[path] = id;

    return id;
}

GLuint ShaderManager::link_program(GLuint vert, GLuint frag) {
    GLuint id = glCreateProgram();

    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);

    GLint result = GL_FALSE;
    int info_log_len;

    glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result != GL_TRUE){
        std::cout << "Error linking program" << std::endl;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetProgramInfoLog(id, info_log_len, NULL, log);
        std::cout << log;

        return 0;
	}

    glDetachShader(id, vert);
    glDetachShader(id, frag);

    return id;
}

void ShaderManager::update_mesh_bufs(GLuint shader, Mesh mesh) {
    // Update the vertex attributes (the connection between the mesh's buffer and the shaders)

    GLint vertexposition = glGetAttribLocation(shader, "e33_vertexposition");
    GLint vertexuv       = glGetAttribLocation(shader, "e33_vertexuv");
    GLint vertexnormal   = glGetAttribLocation(shader, "e33_vertexnormal");
    GLint vertexcolor    = glGetAttribLocation(shader, "e33_vertexcolor");

    glBindVertexArray(mesh.vao);

    // TODO: this is ugly
    if (vertexposition != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.glbufs.vertices_buffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), &mesh.vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            vertexposition,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (vertexuv != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.glbufs.uvs_buffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size() * sizeof(GLfloat), &mesh.uvs[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            vertexuv,
            2,        // vec2
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (vertexnormal != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.glbufs.normals_buffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(GLfloat), &mesh.normals[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            vertexnormal,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (vertexcolor != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.glbufs.colors_buffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.colors.size() * sizeof(GLfloat), &mesh.colors[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
            vertexcolor,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // offset
        );
    }

    if (mesh.indices.size()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.glbufs.indices_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), &mesh.indices[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
