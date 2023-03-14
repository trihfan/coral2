#include "Object.h"
#include "ObjectManager.h"
#include "Engine.h"
#include "utils/Logs.h"

using namespace coral;

Object::Object() :
    state(InitState::notInitialized), willReset(false)
{
}

Object::~Object()
{
    if (state == InitState::initialized)
    {
        Logs(warning) << "object '" << name.get() << "' (" << this << ") has not been released";
    }
}

void Object::reset()
{
    if (!willReset)
    {
        engine->objectManager->reset(shared_from_this());
        willReset = true;
    }
}
