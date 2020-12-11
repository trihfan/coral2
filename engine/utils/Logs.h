#ifndef LOGS_H
#define LOGS_H

#include <array>
#include <string>
#include <string_view>
#include <iostream>

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
        Logs(LogType type) : type(type)
        {
            setColor(type);
            static constexpr std::array<std::string_view, 6> headers { "error", "warning", "info", "success", "fail", "debug" };
            write(std::string("[") + std::string(headers[static_cast<size_t>(type)]) + "] ");
        }

        ~Logs()
        {
            *this << "\033[0m" << std::endl;
        }

        // stream operator
        template<typename type>
        inline Logs& operator<<(const type& value)
        {
            write(value);
            return *this;
        }

        // std manip
        typedef std::basic_ostream<char, std::char_traits<char>> coutType;
        typedef coutType& (*coutManipulator)(coutType&);

        inline Logs& operator<<(coutManipulator manip)
        {
            manip(std::cout);
            return *this;
        }

    private:
        const std::array<int, 5> verbosity { -1, 0, 4, 4, 5 };
        LogType type;

        void setColor(LogType type)
        {
#if defined(__linux__) || defined(__APPLE__)
            static const std::array<std::string, 6> colors { "\033[31m", "\033[33m", "\033[36m", "\033[32;1m", "\033[31;1m", "\033[35m" };
            write(colors[static_cast<size_t>(type)]);
#else
            // todo
            static const std::array<std::string, 6> colors{ "\033[31m", "\033[33m", "\033[36m", "\033[32;1m", "\033[31;1m", "\033[35m" };
            write(colors[static_cast<size_t>(type)]);
#endif
        }

        template<typename type>
        void write(const type& value)
        {
            std::cout << value;
        }
    };
}

#endif