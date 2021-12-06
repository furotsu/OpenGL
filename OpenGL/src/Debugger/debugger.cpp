#include <iostream>

#include "debugger.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

void LogCall(const char* tag, std::string log)
{
    std::cout << "[Log call] " << tag << ": " << log << std::endl;
}

void ErrorLog(std::string str, const char* file, int line)
{
    std::cout << "[Error] (" << str << ")" << file << ":" << line << std::endl;
}

bool LogAssert(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}