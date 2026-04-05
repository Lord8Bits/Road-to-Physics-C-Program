//
// Created by lorend on 11/27/25.
//
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <print>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{   // Necessary to bridge the program and the GPU driver
    glfwInit();

    /*
     * glfwWindowHint is used to specify the template for the next glfwCreateWindow call
     * since GLFW acts like a state machine, you will add hints that are applied
     * when the next window is created. If the window was already created, changing the hints
     * won't affect the existing window.
    */

    // Version MAJOR 3 and Version MINOR 3 means we are using opengl v3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // We will use the OpenGL more modern Core Profile:
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creating the window (Finally!) :
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearningOpenGL", NULL, NULL);
    // Checking the window has been generated successfully
    if (window == NULL) {
        std::print("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    // Display the window
    glfwMakeContextCurrent(window);

    // Now, we have to init GLAD so that we could use the GPU driver with openGL functions :
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::print("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }
    // Then, we will specify what we call Viewport, basically a coordinate system for our window:
    glViewport(0, 0, 800, 600);

    /*
     * Unfortunately, if we resize the window our viewport will be messy,
     * rendering everything on a smaller window inside a large one,
     * so we use glfwSetFramebufferSizeCallback to update the viewport each time the window is resized.
    */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // AT LAST (fr this time), we will make the loop to actually open the window not instantly close:
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // To stop any communication with the OS and GPU driver
    glfwTerminate();

    return 0;
}