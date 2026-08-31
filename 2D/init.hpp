//
// Created by lorend on 8/29/26.
//

#pragma once
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
class OpenGLContext {
private:
    int _version_major;
    int _version_minor;
    int _profile;
public:
    bool init_GLFW();
    OpenGLContext(int version_major,
               int version_minor,
               int profile)
                   : _version_major(version_major),
                    _version_minor(version_minor),
                    _profile(profile){}
    ~OpenGLContext()
    {
        glfwTerminate();
    }
    OpenGLContext(const OpenGLContext&) = delete;
    OpenGLContext& operator=(const OpenGLContext&) = delete;

    GLFWwindow* init_window(int width,
                            int height,
                            const char* title,
                            GLFWframebuffersizefun callback);

    bool init_GL(int width, int height);
};



