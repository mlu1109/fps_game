#include <GL/glew.h>
#include <string>
#include <iostream>
#include "Error.hpp"


GLenum lastError;
std::string lastMessage;

void printError(const std::string &message)
{
   GLenum error = glGetError();
   while (error != GL_NO_ERROR)
   {
       if (lastError != error || lastMessage != message)
       {
           std::cerr << "GL error 0x" << std::hex << error << " detected. Given message: " << message << '\n';
	       lastMessage = message;
	       lastError = error;
       }
       error = glGetError();
   }
}
