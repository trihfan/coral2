#include "ObjectFactory.h"
#include "Handle.h"
#include "Object.h"
#include "ObjectFactoryData.h"
#include "utils/Logs.h"
#include <algorithm>

using namespace coral;

std::atomic<uint64_t> ObjectFactory::counter(0);
bool ObjectFactoryData::isDestroyed = false;
#define internalData ObjectFactoryData::get()

DEFINE_SINGLETON(ObjectFactoryData)
DEFINE_SINGLETON(ObjectFactory)

void ObjectFactoryData::release()
{
    isDestroyed = true;
}

ObjectFactory::ObjectFactory()
{
    ObjectFactoryData::create();
}

void ObjectFactory::release()
{
    // Destroy all objects
    for (const auto& object : internalData->objects)
    {
        internalData->releaseList.enqueue(object);
    }
    update();

    ObjectFactoryData::destroy();
}

void ObjectFactory::add(Handle<Object> object)
{
    internalData->initializeList.enqueue(object);
}

void ObjectFactory::remove(Handle<Object> object)
{
    // Move to release list
    internalData->releaseList.enqueue(object);
}

void ObjectFactory::update()
{
    // Initilize objects
    Handle<Object> object;
    while (internalData->initializeList.try_dequeue(object))
    {
        if (*object && object->state == Object::ObjectState::not_initialized)
        {
            object->init();
            object->state = Object::ObjectState::initialized;

            // Add to object pool
            uint64_t index = object.sharedMemory->index;
            if (index >= internalData->objects.size())
            {
                internalData->objects.resize(index);
            }
            internalData->objects[index] = object;
        }
    }

    // Release objects
    std::vector<std::pair<HandleSharedMemory*, Object*>> toDelete;
    while (internalData->releaseList.try_dequeue(object))
    {
        if (*object && object->state == Object::ObjectState::initialized)
        {
            object->release();
            object->state = Object::ObjectState::released;

            // Unallocate
            toDelete.push_back(std::make_pair(object.sharedMemory, object.data));

            // Remove from object pool
            uint64_t index = object.sharedMemory->index;
            internalData->objects[index] = nullptr;
        }
    }

    // Deallocate
    object = nullptr;
    for (const auto& pair : toDelete)
    {
        delete pair.first;
        delete pair.second;
    }
}