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
#include "mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

    shadermanager = ShaderManager();
    GLuint my_shader_id = shadermanager.link_program(
        shadermanager.load_shader(GL_VERTEX_SHADER, "../shaders/mvp_uv.vert"),
        shadermanager.load_shader(GL_FRAGMENT_SHADER, "../shaders/texture.frag")
    );

    ShaderProgram my_shader(my_shader_id);

    Mesh my_mesh;
    my_mesh.load_wfobj("../models/f15.obj");
    my_mesh.update_mesh_bufs(my_shader);

    int texw, texh, nchans;
    unsigned char * my_texture_data = stbi_load("../textures/f15.png", &texw, &texh, &nchans, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texture_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    cam = Camera(glm::vec3(0.0f, 0.0f, -5.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glfwSwapInterval(0); // Disable VSync

    glfwSetTime(0);
    seconds = glfwGetTime();

    size_t i = 0;
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen

        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE0);

        my_mesh.render(my_shader);

        glfwSwapBuffers(window);

        double new_seconds = glfwGetTime();
        deltaT = new_seconds - seconds;
        seconds = new_seconds;

        handle_keyboard();
        handle_mouse();

        i++;
        if (i % 240 == 0)
            //printf("FPS = %f\n", 1/deltaT);
            printf("FPS = %f\n", i/seconds); // For average

    } while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    return 0;
}
