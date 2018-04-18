#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Shader.hpp"

std::string readFile(const std::string &path)
{
    std::ifstream fs;
    fs.open(path.c_str());
    if (!fs)
        throw std::runtime_error("readFile failed file not found: " + path);

    std::stringstream ss;
    ss << fs.rdbuf();
    fs.close();
    return ss.str();
}

std::string getShaderInfoLog(GLuint shader)
{
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
    return std::string(infoLog.data());
}

std::string getProgramInfoLog(GLuint program)
{
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
    return std::string(infoLog.data());
}

/*
 * https://www.khronos.org/opengl/wiki/Shader_Compilation#Examples_of_separate_programs
 */

GLuint loadShaders(const std::string &vertPath, const std::string &fragPath)
{
    std::string vertexSource = readFile(vertPath);
    std::string fragmentSource = readFile(fragPath);

    /* Vertex Shader */
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *source = reinterpret_cast<const GLchar *>(vertexSource.c_str());
    glShaderSource(vertexShader, 1, &source, 0);
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        std::cout << getShaderInfoLog(vertexShader) << '\n';
        glDeleteShader(vertexShader);
        return 0;
    }

    /* Fragment Shader */
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source = (const GLchar *)fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);
    glCompileShader(fragmentShader);

    isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        std::cout << getShaderInfoLog(fragmentShader) << '\n';
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return 0;
    }

    /* Program */
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        std::cout << getProgramInfoLog(program) << '\n';
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

Shader::Shader(const std::string &vertPath, const std::string &fragPath)
{
    m_program = loadShaders(vertPath, fragPath);
}