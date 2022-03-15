#include "ObjectManager.h"
#include "Object.h"
#include "utils/Logs.h"
#include <algorithm>

using namespace coral;

ObjectManager::ObjectManager()
{
    objects.reserve(1000);
    freePositions.reserve(100);
}

ObjectManager::~ObjectManager()
{
    std::vector<Handle<Object>> toRelease;
    toRelease.reserve(objects.size());
    for (auto& object : objects)
    {
        if (object)
        {
            toRelease.push_back(object);
        }
    }
    releaseList.enqueue_bulk(toRelease.begin(), toRelease.size());
    update();
}

void ObjectManager::update()
{
    Handle<Object> object;

    // Object to reset
    while (resetList.try_dequeue(object))
    {
        if (object->state != Object::InitState::initialized)
        {
            Logs(error) << "Wrong reset call";
        }

        object->release();
        object->init();
        object->state = Object::InitState::initialized;
        object = nullptr;
    }

    // Objects to init
    while (initializeList.try_dequeue(object))
    {
        if (object->state != Object::InitState::notInitialized)
        {
            Logs(error) << "multiple initialization";
        }

        object->init();
        object->state = Object::InitState::initialized;

        // Insert the object
        if (!freePositions.empty())
        {
            objects[freePositions.back()] = object;
            freePositions.pop_back();
        }
        else
        {
            objects.push_back(object);
        }
        object = nullptr;
    }

    // Objects to release
    while (releaseList.try_dequeue(object))
    {
        if (object->state != Object::InitState::initialized)
        {
            Logs(error) << "Multiple deletion";
        }

        object->release();
        object->state = Object::InitState::released;
        object = nullptr;
    }
}

void ObjectManager::add(const Handle<Object>& object)
{
    initializeList.enqueue(object);
}

void ObjectManager::remove(const Handle<Object>& object)
{
    releaseList.enqueue(object);
}

void ObjectManager::reset(const Handle<Object>& object)
{
    resetList.enqueue(object);
}
