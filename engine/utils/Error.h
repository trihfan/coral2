#ifndef ERROR_H
#define ERROR_H

#include "glad/glad.h"
#include <string>

#define CHECK_OPENGL_ERROR coral::OpenglError::check(std::string(__FILE__), std::string(__PRETTY_FUNCTION__), std::to_string(__LINE__));

namespace coral
{
class OpenglError
{
public:
    static void check(const std::string& file, const std::string& function, const std::string& line);
};    
}

#endif