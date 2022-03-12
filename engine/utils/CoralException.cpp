#include "CoralException.h"
#include "utils/Logs.h"

using namespace coral;

CoralException::CoralException(const std::string& message)
    : std::runtime_error(message), message(message)
{
    Logs(error) << message;
}
