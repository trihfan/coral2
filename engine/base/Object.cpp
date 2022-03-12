#include "Object.h"
#include "utils/Logs.h"

using namespace coral;

Object::Object()
    : state(ObjectState::not_initialized)
{
}

Object::~Object()
{
    if (state == ObjectState::initialized)
    {
        Logs(warning) << "object '" << getName() << "' has not been released";
    }
}

void Object::setName(const std::string& name)
{
    this->name = name;
}

const std::string& Object::getName() const
{
    return name;
}

ObjectState Object::getState() const
{
    return state;
}

void Object::init()
{
}

void Object::release()
{
}
