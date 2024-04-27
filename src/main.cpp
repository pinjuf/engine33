#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h> // Window/keyboard handling
#include <glm/glm.hpp> // 3D maths
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "main.h"
#include "shaders.h"

GLFWwindow * window;
GLuint vao;

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 8); // Antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(1920, 1080, "engine33", NULL, NULL);

    // Hacky hack because GLFW_FLOATING doesn't work apparently
    system("/bin/i3-msg floating enable");

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window); // "Connect" OpenGL to our window

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Should be changed for better controls

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glGenVertexArrays(1, &vao); // Create and use our VAO
    glBindVertexArray(vao);

    GLuint my_shader = loadShader("../shaders/vp.vert", "../shaders/solid.frag");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glm::mat4 camera = glm::lookAt(
        glm::vec3(3, 3, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(120.0f),
        1920.0f/1080.0f,
        0.1f,
        100.0f
    );

    glm::mat4 vp = projection * camera;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen

        glUseProgram(my_shader);
        glUniform3f(glGetUniformLocation(my_shader, "input_color"), 0.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(my_shader, "mvp"), 1, GL_FALSE, &vp[0][0]);

        glEnableVertexAttribArray(0); // We pass the vertex through attribute #0
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,        // attribute #0
                sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0])/3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized
                0,        // stride
                (void*)0  // offset in buffer
        );

        glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0])/3); // Draw!
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        vp = projection * camera;

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    return 0;
}
