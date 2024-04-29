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
#include "camera.h"
#include "keyboard.h"
#include "mouse.h"

GLFWwindow * window;
PYR_Camera cam;

double seconds;
double deltaT;

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 8); // Antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(WIDTH, HEIGHT, "engine33", NULL, NULL);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!glfwRawMouseMotionSupported()) {
        std::cerr << "Couldn't get raw cursor motion" << std::endl;
        return -1;
    }
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint vao;
    glGenVertexArrays(1, &vao); // Create and use our VAO
    glBindVertexArray(vao);

    GLuint my_shader = loadShader("../shaders/mvp.vert", "../shaders/solid.frag");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    cam = PYR_Camera(glm::vec3(3, 3, 3), -glm::pi<float>()/4, glm::pi<float>() * 0.75f);
    glm::mat4 vp;

    GLuint shader_vertex_pos = glGetAttribLocation(my_shader, "vertpos_model");

    glEnable(GL_DEPTH_TEST);

    glfwSetTime(0);
    seconds = glfwGetTime();

    do {
        vp = cam.vpmatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen

        glUseProgram(my_shader);
        glUniform3f(glGetUniformLocation(my_shader, "input_color"), 0.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(my_shader, "mvp"), 1, GL_FALSE, &vp[0][0]);

        glEnableVertexAttribArray(shader_vertex_pos);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                shader_vertex_pos,
                3,
                GL_FLOAT, // type
                GL_FALSE, // normalized
                0,        // stride
                (void*)0  // offset in buffer
        );

        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw!

        glUniform3f(glGetUniformLocation(my_shader, "input_color"), 1.0f, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 3, 6); // Draw!

        glDisableVertexAttribArray(shader_vertex_pos);

        glfwSwapBuffers(window);

        double new_seconds = glfwGetTime();
        deltaT = new_seconds - seconds;
        seconds = new_seconds;

        handle_keyboard();
        handle_mouse();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    return 0;
}
