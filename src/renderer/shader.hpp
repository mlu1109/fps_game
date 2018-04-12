#pragma once 

#include <string>
#include <GL/glew.h>
#include <unordered_map>

struct Shader
{
    GLuint program;
    std::string vertPath;
    std::string fragPath;
};

Shader initShaders(); // TODO: Extend