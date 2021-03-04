#include "OpenglError.h"
#include "Logs.h"
#include "gl.h"

using namespace coral;
using namespace backend::opengl;

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
#ifdef GL_STACK_OVERFLOW
    case GL_STACK_OVERFLOW:
        return "Stack overflow";
#endif
#ifdef GL_STACK_UNDERFLOW
    case GL_STACK_UNDERFLOW:
        return "Stack underflow";
#endif
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
