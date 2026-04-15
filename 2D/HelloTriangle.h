#pragma once
#include "include/glad/glad.h"

struct Triangle2DMesh {
    GLuint VAO;
    GLuint VBO;
    GLuint shaderProgram;
    GLint uTime;
};

void bindVBO(GLuint &VBO, float *vertices, size_t totalSize);

GLuint compileTriangleShaders();

void linkVertexAttributes(GLuint &VAO, GLuint &VBO, float *vertices, size_t totalSize);

Triangle2DMesh initializeTriangle(float* vertices, size_t totalSize);

void cleanupTriangle(Triangle2DMesh &triangle);