#pragma once

#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include "Error.hpp"

class Shader
{
    GLuint m_program;

  public:
    Shader(const std::string &vertPath, const std::string &fragPath);
    
    void destroy() { glDeleteProgram(m_program); }
    void bind() const;
};
