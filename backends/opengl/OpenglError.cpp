#include "OpenglError.h"
#include "Logs.h"

using namespace coral;

const char* getGLErrorStr(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:
        return "No error";
    case GL_INVALID_ENUM:
        return "Invalid enum";
    case GL_INVALID_VALUE:
        return "Invalid value";
    case GL_INVALID_OPERATION:
        return "Invalid operation";
    case GL_STACK_OVERFLOW:
        return "Stack overflow";
    case GL_STACK_UNDERFLOW:
        return "Stack underflow";
    case GL_OUT_OF_MEMORY:
        return "Out of memory";
    default:
        return "Unknown error";
    }
}

void OpenglError::check(const std::string& file, const std::string& function, const std::string& line)
{
    while (true)
    {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;

        Logs(error) << "gl error " << getGLErrorStr(err) << " in function " << function << ", in file " << file << " at line " << line;
    }
}
