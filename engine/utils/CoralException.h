#pragma once
#include <stdexcept>
#include <string>

namespace coral
{
    class CoralException : public std::runtime_error
    {
    public:
        CoralException(const std::string& message);

    private:
        std::string message;
    };
}
