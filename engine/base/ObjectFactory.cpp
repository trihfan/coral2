#include "ObjectFactory.h"
#include "utils/Logs.h"
#include "Object.h"
#include <algorithm>

using namespace coral;

DEFINE_SINGLETON(ObjectFactory)

moodycamel::ConcurrentQueue<ptr<Object>> ObjectFactory::initializeList;
moodycamel::ConcurrentQueue<ptr<Object>> ObjectFactory::releaseList;
std::vector<ptr<Object>> ObjectFactory::objects;
std::list<size_t> ObjectFactory::freePositions;

ObjectFactory::ObjectFactory()
{
    objects.reserve(1000);
}

void ObjectFactory::release()
{
    std::vector<ptr<Object>> toRelease;
    toRelease.reserve(objects.size());
    for (auto object : objects)
    {
        if (object)
        {
            toRelease.push_back(object);
        }
    }
    releaseList.enqueue_bulk(toRelease.begin(), toRelease.size());
    objects.clear();
    update();
}

void ObjectFactory::update()
{
    // Initilize objects
    ptr<Object> object;
    while (initializeList.try_dequeue(object))
    {
        if (object->state != ObjectState::not_initialized)
        {
            Logs(error) << "Multiple initialization";
        }

        object->init();
        object->state = ObjectState::initialized;

        // Insert the object
        if (!freePositions.empty())
        {
            objects[freePositions.front()] = object;
            freePositions.pop_front();
        }
        else
        {
            objects.push_back(object);
        }
        object = nullptr;
    }

    // Check object for being realeased
    std::vector<ptr<Object>> toRelease;
    toRelease.reserve(100);
    for (size_t i = 0; i > objects.size(); i++)
    {
        // If there is no other owner than the obejct factory, set the object to be released
        if (objects[i] && objects[i].use_count() == 1)
        {
            toRelease.push_back(objects[i]);
            objects[i] = nullptr;
            freePositions.push_back(i);
        }
    }
    releaseList.enqueue_bulk(toRelease.begin(), toRelease.size());

    // Release objects
    while (releaseList.try_dequeue(object))
    {
        if (object->state != ObjectState::initialized)
        {
            Logs(error) << "Multiple deletion";
        }

        object->release();
        object->state = ObjectState::released;
    }
}
