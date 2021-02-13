#include "ObjectManager.h"
#include "utils/Logs.h"
#include <algorithm>

using namespace coral;

DEFINE_SINGLETON(ObjectManager)

void ObjectManager::release()
{
    for (auto object : objects)
    {
        if (object->state == Object::ObjectState::initialized)
        {
            instance->releaseList.push_back(object);
        }
    }
    for (auto object : instance->releaseList)
    {
        object->release();
        object->state = Object::ObjectState::released;
    }

    objects.clear();
    instance->initializeList.clear();
    instance->releaseList.clear();
}

void ObjectManager::destroy(std::shared_ptr<Object> object)
{
    instance->unregisterObject(object);
}

ObjectManager::ObjectManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
    : memory_resource(memory_resource)
{
}

void ObjectManager::registerObject(std::shared_ptr<Object> object)
{
    objects.push_back(object);
    instance->initializeList.push_back(object);
}

void ObjectManager::unregisterObject(std::shared_ptr<Object> object)
{
    objects.erase(std::find(objects.begin(), objects.end(), object));
    instance->releaseList.push_back(object);
}

void ObjectManager::update()
{
    for (auto object : instance->initializeList)
    {
        if (object->state == Object::ObjectState::not_initialized)
        {
            object->init();
            object->state = Object::ObjectState::initialized;
        }
    }

    for (auto object : instance->releaseList)
    {
        if (object->state == Object::ObjectState::initialized)
        {
            object->release();
            object->state = Object::ObjectState::released;
        }
    }

    instance->initializeList.clear();
    instance->releaseList.clear();
}