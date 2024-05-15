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
#include "autoload.h"

GLFWwindow * window;

ShaderManager shadermanager;
AutoLoader autoloader;
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

    window = glfwCreateWindow(WIDTH, HEIGHT, "engine33", glfwGetPrimaryMonitor(), NULL);

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

    ShaderProgram plane_shader(shadermanager.link_program(
        shadermanager.load_shader(GL_VERTEX_SHADER, "../shaders/mvp_uv.vert"),
        shadermanager.load_shader(GL_FRAGMENT_SHADER, "../shaders/texture.frag")
    ));

    ShaderProgram terrain_shader(shadermanager.link_program(
        shadermanager.load_shader(GL_VERTEX_SHADER, "../shaders/mvp_uv.vert"),
        shadermanager.load_shader(GL_FRAGMENT_SHADER, "../shaders/texture.frag")
    ));

    autoloader.load_loadfile("../loadfiles/fs.load");

    cam = Camera();
    cam.p.position = BACKWARD * 3.0f + UP * 1.0f;
    cam.p.parent = &(autoloader.objects["plane"]->p);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glfwSwapInterval(0); // Disable VSync

    glfwSetTime(0);
    seconds = glfwGetTime();

    size_t i = 0;
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen

        autoloader.render();

        glfwSwapBuffers(window);

        double new_seconds = glfwGetTime();
        deltaT = new_seconds - seconds;
        seconds = new_seconds;

        glfwPollEvents();

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
