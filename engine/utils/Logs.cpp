#include "Logs.h"
#include "rang.hpp"

using namespace coral;

Logs::Logs(LogType type) : type(type)
{
    setColor(type);
    static constexpr std::array<std::string_view, 6> headers { "error", "warning", "info", "success", "fail", "debug" };
    write(std::string("[") + std::string(headers[static_cast<size_t>(type)]) + "] ");
}

Logs::~Logs()
{
    *this << rang::style::reset << rang::fg::reset << std::endl;
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
        write(rang::fg::red);
        break;

    case warning:
        write(rang::fg::yellow);
        break;

    case info:
        write(rang::fg::cyan);
        break;

    case success:
        write(rang::fg::green);
        write(rang::style::bold);
        break;

    case fail:
        write(rang::fg::red);
        write(rang::style::bold);
        break;

    case debug:
        write(rang::fg::magenta);
        break;
    }
}