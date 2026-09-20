#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "init.hpp"
#include "ShaderProgram.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 800;

int main()
{
    /*
     * This part initializes the glfw library and GLAD loader.
     * It specifies the window parameters aka hint. (in this case which opengl version)
     * Create the pointer of the window using glfwCreateWindow function.
     * Specify the current context in this case the newly created window.
     * Load the OpenGL functions addresses by asking the driver using glfwGetProcAddress.
     * And map the pixels with the OpenGL coordinates using framebuffer function that gets called when resized.
     */

    OpenGLContext init(3 , 3, GLFW_OPENGL_CORE_PROFILE);

    if (!init.init_GLFW()) return -1;
    GLFWwindow *window {init.init_window(WIDTH, HEIGHT, "Hello Triangle", framebuffer_size_callback)};
    if (window == nullptr) return -2;

    if (!init.init_GL(WIDTH, HEIGHT)) return -1;

    /*
     * This section prepares the GPU memory to send vertices in what we call VBOs.
     * By first creating the vertices data using an array such as the triangle array,
     * we then create the VAO which defines how to read the vertex data that we will send,
     * and the VBO, where the vertices will be stored.
     * You create 2 empty arrays of type unsigned int for the VBO and VAO.
     * Generate IDs for the VBO and VAO using glGen* functions.
     * Bind those IDs to specify which VBO or VAO to alter in the GPU.
     * Send the vertices data to the VBO by also specifying the size and which type of draw.
     * And finally, set the parameters in the VAO that specify how the driver should read the data,
     * and enable the vertex attribute.
     * All of this will enable us to use the vertices in a shader program,
     * which is where the calculations for lighting, shadows, colors and how the pixels react are.
     */
    std::vector<float> triangle{
        0.5f , -0.5f , 0.f,
        0.f  , 0.5f  , 0.f,
        -0.5f, -0.5f , 0.f
    };

    std::vector<GLuint> VAOs(1);
    glGenVertexArrays(1, VAOs.data());
    glBindVertexArray(VAOs[0]);

    std::vector<GLuint>  VBOs(1);
    glGenBuffers(1, VBOs.data());
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    glBufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(triangle.size() * sizeof(float)),
        triangle.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    ShaderProgram hello_triangle_shader("HelloTriangle", "shaders");

    GLint uTimeLoc = glGetUniformLocation(hello_triangle_shader.getShaderProgram(), "uTime");

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        processInput(window);

        glClearColor(0.545f, 0.0f, 0.545f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        hello_triangle_shader.useProgram();
        glUniform1f(uTimeLoc, static_cast<float>(currentTime));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, VAOs.data());
    glDeleteBuffers(1, VBOs.data());

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow *window)
{
    // Detects if esc has been pressed, if so, close the window:
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}