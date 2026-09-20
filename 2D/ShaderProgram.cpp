//
// Created by lorend on 8/31/26.
//

#include "ShaderProgram.hpp"

#include <iostream>

ShaderProgram::ShaderProgram(const char *shader_filename, const char* shader_dir)
{
    _shader_dir = shader_dir;
    _shader_name = shader_filename;

    findAndReadShaders();

    GLuint vertex_shader = createShader(GL_VERTEX_SHADER, _vertex_shader_sources);
    GLuint fragment_shader = createShader(GL_FRAGMENT_SHADER, _fragment_shader_sources);

    linkProgram(vertex_shader, fragment_shader);
}

void ShaderProgram::findAndReadShaders()
{
    if (!fs::exists(_shader_dir)) {
        std::cerr << "Error: missing " << fs::absolute(_shader_dir) << " directory!\n";
        return;
    }

    try {
        for (const auto& entry : fs::directory_iterator(_shader_dir)) {
            if (fs::is_regular_file(entry)) {
                // Check if the filename stem matches the target shader name (e.g., "HelloTriangle")
                if (entry.path().stem() == _shader_name) {

                    if (entry.path().extension() == ".vrtxs") {
                        std::string code = readFileContents(entry.path());
                        if (!code.empty()) {
                            _vertex_shader_sources.push_back(code);
                        }
                    }
                    else if (entry.path().extension() == ".frags") {
                        std::string code = readFileContents(entry.path());
                        if (!code.empty()) {
                            _fragment_shader_sources.push_back(code);
                        }
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }
}


std::string ShaderProgram::readFileContents(const std::ifstream &source)
{
    std::stringstream buffer{};
    buffer << source.rdbuf();
    return buffer.str();
}

GLuint ShaderProgram::createShader(GLenum shaderType, const std::vector<std::string>& sources)
{
    const char* versionHeader = "#version 330 core\n";
    std::vector<const char*> c_strings;

    // Always push the version header first
    c_strings.push_back(versionHeader);

    // Collect const char* pointers from our stored string vector
    for (const auto& code : sources) {
        c_strings.push_back(code.c_str());
    }

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, static_cast<GLsizei>(c_strings.size()), c_strings.data(), nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED (Type: " << shaderType << ")\n"
                  << infoLog << '\n';
    }

    return shader;
}

void ShaderProgram::linkProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    _shader_program = glCreateProgram();
    glAttachShader(_shader_program, vertex_shader);
    glAttachShader(_shader_program, fragment_shader);
    glLinkProgram(_shader_program);

    int success;
    char infoLog[512];
    glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shader_program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

ShaderProgram::~ShaderProgram()
{
    if (_shader_program != 0) {
        glDeleteProgram(_shader_program);
    }
}

void ShaderProgram::useProgram() const
{
    glUseProgram(_shader_program);
}
