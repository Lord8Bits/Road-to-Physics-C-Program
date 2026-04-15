//
// Created by lorend on 11/27/25.
//
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <print>
#include "HelloTriangle.h"
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 600;

int main()
{   // Necessary to bridge the program and the GPU driver
    glfwInit();

    /*
     * glfwWindowHint is used to specify the template for the next glfwCreateWindow call
     * since GLFW acts like a state machine, you will add hints that are applied
     * when the next window is created. If the window has already been created, changing the hints
     * won't affect the existing window.
    */

    // Version MAJOR 3 and Version MINOR 3 means we are using opengl v3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // We will use the OpenGL more modern Core Profile:
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creating the window (Finally!) :
    GLFWwindow* window{glfwCreateWindow(WIDTH, HEIGHT, "LearningOpenGL", NULL, NULL)};
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
    glViewport(0, 0, WIDTH, HEIGHT);

    /*
     * Unfortunately, if we resize the window our viewport will be messy,
     * rendering everything on a smaller window inside a large one.
     * So we use glfwSetFramebufferSizeCallback to update the viewport each time the window is resized.
    */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // calls the framebuffer_size_callback function if resized

    // Triangle init:
    float vertices[9] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };


    Triangle2DMesh triangle1{initializeTriangle(vertices, sizeof(vertices))};

    double currentTime{};
    // AT LAST (fr this time), we will make the loop to actually leave the window open and not instantly close:
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        // Input:
        processInput(window); // Always add events like key presses before glfwPollEvents

        // Render:
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(triangle1.shaderProgram);
        glUniform1f(triangle1.uTime, static_cast<float>(currentTime));
        glBindVertexArray(triangle1.VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Process events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // To stop any communication with the OS and GPU driver
    glfwTerminate();

    cleanupTriangle(triangle1);


    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Updates Viewport
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // Detects if esc has been pressed, if so, close the window:
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
