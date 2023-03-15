#include "ObjectInterface.h"
#include "ObjectManager.h"
#include "utils/Logs.h"

using namespace coral;

ObjectInterface::ObjectInterface() :
    state(InitState::notInitialized), willReset(false)
{
}

ObjectInterface::~ObjectInterface()
{
    if (state == InitState::initialized)
    {
        Logs(warning) << "object '" << name.get() << "' (" << this << ") has not been released";
    }
}

void ObjectInterface::reset()
{
    if (!willReset)
    {
        getHandle<ObjectInterface>().reset();
        willReset = true;
    }
}
