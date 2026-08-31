#include "init.hpp"
#include <print>

bool OpenGLContext::init_GLFW()
{
    if (!glfwInit()) {
        std::print("ERROR: Failed to initialize GLFW\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, _profile);

    return true;
}

GLFWwindow* OpenGLContext::init_window(int width, int height, const char* title, GLFWframebuffersizefun callback)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::print(stderr, "ERROR: Failed to create GLFW window\n");
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (callback) {
        glfwSetFramebufferSizeCallback(window, callback);
    }
    return window;
}

bool OpenGLContext::init_GL(int width, int height)
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::print("ERROR: Failed to Initialize GLAD.\n");
        return false;
    }

    glViewport(0,0, width, height);
    return true;
}

