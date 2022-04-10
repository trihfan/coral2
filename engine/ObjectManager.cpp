#include "ObjectManager.h"
#include "Object.h"
#include "ObjectInterface.h"
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
    std::vector<Object<ObjectInterface>> toRelease;
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
    Object<ObjectInterface> object;

    // Object to reset
    while (resetList.try_dequeue(object))
    {
        if (object->state != InitState::initialized)
        {
            Logs(error) << "Reset call on not inialized object";
            continue;
        }

        object->release();
        object->init();
        object->state = InitState::initialized;
    }
    object = nullptr;

    // Objects to init
    while (initializeList.try_dequeue(object))
    {
        if (object->state == InitState::initialized)
        {
            Logs(error) << "Multiple initialization";
            continue;
        }

        object->init();
        object->state = InitState::initialized;

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
    }
    object = nullptr;

    // Objects to release
    while (releaseList.try_dequeue(object))
    {
        if (object->state != InitState::initialized)
        {
            Logs(error) << "Multiple deletion";
            continue;
        }

        object->release();
        object->state = InitState::released;
    }
    object = nullptr;
}

void ObjectManager::add(const Object<ObjectInterface>& object)
{
    initializeList.enqueue(object);
}

void ObjectManager::remove(const Object<ObjectInterface>& object)
{
    releaseList.enqueue(object);
}

void ObjectManager::reset(const Object<ObjectInterface>& object)
{
    resetList.enqueue(object);
}
