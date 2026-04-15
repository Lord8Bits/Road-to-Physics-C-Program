//
// Created by lorend on 4/6/26.
//
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "HelloTriangle.h"
#include <print>

void bindVBO(GLuint &VBO, float *vertices, const size_t totalSize)
{
    // Create a vertex buffer object:
    glGenBuffers(1, &VBO);

    // Bind the newly created VBO(s) to the port where they reside:
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    // Copy the vertices data to the VBO port residing in the VRAM:
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(totalSize), vertices, GL_STATIC_DRAW);
}

GLuint compileTriangleShaders()
{
    // We are creating the vertex shader referenced by an ID using glCreateShader:
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};

    // Here we would compile the shader at run-time using GLSL a sort of OpenGL programming language similar to C:
    const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // This is to verify whether the compiling was successful
    int success{};
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}" , infoLog);
    }

    // Here we create the fragment shader which calculates the resulting colors of the objects:
    const char* fragmentShaderSource = R"(#version 330 core
out vec4 FragColor;
uniform float uTime;

void main()
{
    float r = sin(uTime) * 0.5f + 0.5f;
    float g = sin(uTime + 2.09f) * 0.5f + 0.5f;
    float b = sin(uTime + 4.18f) * 0.5f + 0.5f;

    FragColor = vec4(r, g, b, 1.0f);
} )";

    unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    infoLog[0] = '\0';
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL,infoLog);
        std::print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}" , infoLog);
    }

    // We are creating the shader program that will combine both shaders and render them:
    unsigned int shaderProgram{glCreateProgram()};
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    infoLog[0] = '\0';
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}" , infoLog);
    }

    // Let's not forget to delete the stored shaders objects in the VRAM since we no longer need them.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void linkVertexAttributes(GLuint &VAO, GLuint &VBO, float *vertices, size_t totalSize)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    bindVBO(VBO, vertices, totalSize);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Triangle2DMesh initializeTriangle(float* vertices, const size_t totalSize)
{
    Triangle2DMesh triangle{};
    linkVertexAttributes(triangle.VAO, triangle.VBO, vertices, totalSize);

    triangle.shaderProgram = compileTriangleShaders();

    triangle.uTime = glGetUniformLocation(triangle.shaderProgram, "uTime");

    return triangle;
}

void cleanupTriangle(Triangle2DMesh &triangle)
{
    glDeleteVertexArrays(1, &triangle.VAO);
    glDeleteBuffers(1, &triangle.VBO);
    glDeleteProgram(triangle.shaderProgram);
}
