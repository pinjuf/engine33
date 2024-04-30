#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h> // Window/keyboard handling
#include <glm/glm.hpp> // 3D maths
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "main.h"
#include "shader.h"
#include "camera.h"
#include "keyboard.h"
#include "mouse.h"

GLFWwindow * window;

ShaderManager shadermanager;
Camera cam;

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

    shadermanager = ShaderManager();
    GLuint my_shader = shadermanager.link_program(
        shadermanager.load_shader(GL_VERTEX_SHADER, "../shaders/mvp.vert"),
        shadermanager.load_shader(GL_FRAGMENT_SHADER, "../shaders/solid_interp.frag")
    );

    GLfloat vertex_pos_data[] = {
       -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       0.0f,  1.0f, -1.0f,
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
       -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, 1.0f,
       0.0f,  1.0f, 1.0f,
    };

    GLfloat vertex_color_data[] = {
        1.0f,  0.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f,  0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f,  0.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    GLuint indices_data[] = {
        6, 7, 8,
        0, 1, 2,
        3, 4, 5,
        0, 1, 3,
        1, 3, 4,
        3, 6, 5,
        5, 8, 6,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_pos_data), vertex_pos_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color_data), vertex_color_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint indices;
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_data), indices_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao); // Create and use our VAO

    cam = Camera(glm::vec3(0.0f, 0.5f, -1.5f));
    glm::mat4 vp;

    GLuint shader_vertex_pos = glGetAttribLocation(my_shader, "vertpos_model");
    GLuint shader_color_pos = glGetAttribLocation(my_shader, "vertex_color");

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            shader_vertex_pos,
            3,
            GL_FLOAT, // type
            GL_FALSE, // normalized
            0,        // stride
            (void*)0  // offset in buffer
    );

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
            shader_color_pos,
            3,        // vec3
            GL_FLOAT, // type
            GL_FALSE, // normalized
            0,        // stride
            (void*)0  // offset in buffer
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    glfwSetTime(0);
    seconds = glfwGetTime();

    size_t i = 0;

    do {
        vp = cam.vpmatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen

        glUseProgram(my_shader);

        glUniformMatrix4fv(glGetUniformLocation(my_shader, "mvp"), 1, GL_FALSE, &vp[0][0]);

        glBindVertexArray(vao);

        glEnableVertexAttribArray(shader_vertex_pos);
        glEnableVertexAttribArray(shader_color_pos);

        //glDrawArrays(GL_TRIANGLES, 0, 9); // Draw!
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
        glDrawElements(
                GL_TRIANGLES,
                sizeof(indices_data)/sizeof(indices_data[0]),
                GL_UNSIGNED_INT,
                (void*)0
        );

        glDisableVertexAttribArray(shader_vertex_pos);
        glDisableVertexAttribArray(shader_color_pos);

        glBindVertexArray(0);

        glfwSwapBuffers(window);

        double new_seconds = glfwGetTime();
        deltaT = new_seconds - seconds;
        seconds = new_seconds;

        handle_keyboard();
        handle_mouse();

        i++;
        if (i % 240 == 0)
            printf("FPS = %f\n", 1/deltaT);
            //printf("FPS = %f\n", i/seconds); // For average

    } while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    return 0;
}
