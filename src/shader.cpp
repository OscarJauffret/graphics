//
// Originally created by Oscar Jauffret on 11/04/2026.
// Extracted into the reusable `graphics` library.
//

#include <GL/glew.h>
#include "graphics/shader.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

std::string readFile(const char* path) {
    std::ifstream f(path);
    if (!f) {
        throw std::runtime_error(std::string("Cannot open shader file: ") + path);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// Returns the compiled shader, or throws with the GL info log.
unsigned int compileStage(GLenum type, const std::string& source, const char* label) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Shader compilation failed (") +
                                 label + "):\n" + infoLog);
    }
    return shader;
}

} // namespace

unsigned int Shader::build(const char* vertexPath, const char* fragmentPath) {
    const std::string vertexCode   = readFile(vertexPath);
    const std::string fragmentCode = readFile(fragmentPath);

    unsigned int vertex   = compileStage(GL_VERTEX_SHADER,   vertexCode,   "VERTEX");
    unsigned int fragment = 0;
    try {
        fragment = compileStage(GL_FRAGMENT_SHADER, fragmentCode, "FRAGMENT");
    } catch (...) {
        glDeleteShader(vertex);
        throw;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        glDeleteProgram(program);
        throw std::runtime_error(std::string("Program linking failed:\n") + infoLog);
    }
    return program;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath) {
    ID = build(vertexPath, fragmentPath);
}

Shader::~Shader() {
    del();
}

Shader::Shader(Shader&& other) noexcept
    : ID(other.ID),
      m_vertexPath(std::move(other.m_vertexPath)),
      m_fragmentPath(std::move(other.m_fragmentPath)) {
    other.ID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        del();
        ID = other.ID;
        m_vertexPath = std::move(other.m_vertexPath);
        m_fragmentPath = std::move(other.m_fragmentPath);
        other.ID = 0;
    }
    return *this;
}

bool Shader::reload() {
    try {
        unsigned int program = build(m_vertexPath.c_str(), m_fragmentPath.c_str());
        del();
        ID = program;
        std::cout << "Shader reloaded.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Shader reload failed, keeping previous program:\n"
                  << e.what() << "\n";
        return false;
    }
}

void Shader::use() const { glUseProgram(ID); }

void Shader::del() const {
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& vec) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const std::string& name, const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}
