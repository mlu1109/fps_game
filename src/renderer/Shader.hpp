#pragma once

#include <string>
#include <GL/glew.h>
#include <unordered_map>

class Shader
{
    GLuint m_program;

  public:
    Shader(const std::string &vertPath, const std::string &fragPath);
    Shader(const Shader &) = delete;
    Shader(const Shader &&) = delete;
    ~Shader() { glDeleteProgram(m_program); }
    
    void bind() const { glUseProgram(m_program); }
};