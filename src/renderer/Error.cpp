#include <GL/glew.h>
#include <string>
#include <iostream>
#include "Error.hpp"

const int MAX_ERROR_COUNT = 0;

void printError(const std::string &message)
{
    static int errorCount = 0;
    static GLenum lastError;
    static std::string lastMessage;

    GLenum error = glGetError();
    while (error != GL_NO_ERROR && errorCount < MAX_ERROR_COUNT)
    {
        ++errorCount;
        if (lastError != error || lastMessage != message)
        {
            std::cerr << "GL error 0x" << std::hex << error << " detected. Given message: " << message << '\n';
            lastMessage = message;
            lastError = error;
        }
        error = glGetError();
    }
}
