//
// Created by lorend on 8/31/26.
//

#ifndef ROAD_TO_PHYSICS_C_PROGRAM_SHADERPROGRAM_HPP
#define ROAD_TO_PHYSICS_C_PROGRAM_SHADERPROGRAM_HPP
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;
using ifstreamFilePtr = std::unique_ptr<std::ifstream>;
class ShaderProgram {
    std::vector<std::string> _vertex_shader_sources{};
    std::vector<std::string> _fragment_shader_sources{};
    fs::path _shader_dir;
    std::string_view _shader_name;
    GLuint _shader_program{};

    void findAndReadShaders();

    static std::string readFileContents(const std::ifstream &source);

    static GLuint createShader(GLenum shaderType, const std::vector<std::string> &sources) ;

    void linkProgram(GLuint vertex_shader, GLuint fragment_shader);

public:
    ShaderProgram(const char* shader_filename, const char* shader_dir);


    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void useProgram() const;
    GLuint getShaderProgram() const {return _shader_program;}
};


#endif //ROAD_TO_PHYSICS_C_PROGRAM_SHADERPROGRAM_HPP
