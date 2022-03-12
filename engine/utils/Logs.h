#ifndef LOGS_H
#define LOGS_H

#include <array>
#include <glm/gtx/io.hpp>
#include <iostream>
#include <string>
#include <string_view>

namespace coral
{
    enum LogType : int
    {
        error,
        warning,
        info,
        success,
        fail,
        debug
    };

    class Logs
    {
    public:
        // construction
        Logs(LogType type);
        ~Logs();

        // stream operator
        template <typename type>
        inline Logs& operator<<(const type& value)
        {
            write(value);
            return *this;
        }

        // std manip
        typedef std::basic_ostream<char, std::char_traits<char>> coutType;
        typedef coutType& (*coutManipulator)(coutType&);

        Logs& operator<<(coutManipulator manip);

    private:
        const std::array<int, 5> verbosity { -1, 0, 4, 4, 5 };
        LogType type;

        // set the output color
        void setColor(LogType type);

        // write to the standard output
        template <typename type>
        void write(const type& value)
        {
            std::cout << value;
        }
    };
}

#endif
