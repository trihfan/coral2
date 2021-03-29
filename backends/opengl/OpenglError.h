#pragma once

#include <string>

#ifdef ENABLE_OPENGL_DEBUG
#if _WIN32
#define CHECK_OPENGL_ERROR backend::opengl::OpenglError::check(std::string(__FILE__), std::string(__FUNCSIG__), std::to_string(__LINE__));
#else
#define CHECK_OPENGL_ERROR backend::opengl::OpenglError::check(std::string(__FILE__), std::string(__PRETTY_FUNCTION__), std::to_string(__LINE__));
#endif
#else
#define CHECK_OPENGL_ERROR \
    {                      \
    }
#endif

namespace backend::opengl
{
    class OpenglError
    {
    public:
        static void check(const std::string& file, const std::string& function, const std::string& line);
    };
}
