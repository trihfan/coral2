#include "Logs.h"

using namespace coral;

Logs::Logs(LogType type) : type(type)
{
#ifndef __EMSCRIPTEN__
    setColor(type);
#endif
    static constexpr std::array<std::string_view, 6> headers { "error", "warning", "info", "success", "fail", "debug" };
    write(std::string("[") + std::string(headers[static_cast<size_t>(type)]) + "] ");
}

Logs::~Logs()
{
#ifndef __EMSCRIPTEN__
    *this << "\033[0m" << std::endl;
#else
    *this << std::endl;
#endif
}

Logs& Logs::operator<<(coutManipulator manip)
{
    manip(std::cout);
    return *this;
}

void Logs::setColor(LogType type)
{
    switch (type)
    {
    case error:
        write("\033[31m");
        break;

    case warning:
        write("\033[33m");
        break;

    case info:
        write("\033[36m");
        break;

    case success:
        write("\033[32m");
        break;

    case fail:
        write("\033[31m");
        break;

    case debug:
        write("\033[35m");
        break;
    }
}
