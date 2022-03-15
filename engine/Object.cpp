#include "Object.h"
#include "ObjectManager.h"
#include "utils/Logs.h"

using namespace coral;

ObjectInterface::ObjectInterface() :
    state(InitState::notInitialized)
{
}

ObjectInterface::~ObjectInterface()
{
    if (state == InitState::initialized)
    {
        Logs(warning) << "object '" << name.get() << "' (" << this << ") has not been released";
    }
}
