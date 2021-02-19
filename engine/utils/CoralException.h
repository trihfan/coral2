#pragma once

#include <stdexcept>
#include <string>

namespace coral
{
    class CoralException : public std::runtime_error
    {
    public:
        /**
         * Constructor
         */
        CoralException(const std::string& message);

    private:
        /**
         * The exception message
         */
        std::string message;
    };
}
