#pragma once

#include <string>

#include "OpenGLBackend.h"

#if _WIN32
#define CHECK_OPENGL_ERROR coral::OpenglError::check(std::string(__FILE__), std::string(__FUNCSIG__), std::to_string(__LINE__));
#else
#define CHECK_OPENGL_ERROR coral::OpenglError::check(std::string(__FILE__), std::string(__PRETTY_FUNCTION__), std::to_string(__LINE__));
#endif

namespace coral
{
class OpenglError
{
public:
    static void check(const std::string& file, const std::string& function, const std::string& line);
};
}
