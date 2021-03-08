#include "Handle.h"
#include "HandleImpl.h"
// /\ Must be included before: ||
// || ------------------------ \/
#include "Object.h"
#include "ObjectFactory.h"
#include "ObjectFactoryData.h"
#include "Logs.h"
#include <algorithm>

using namespace coral;

bool ObjectFactoryData::isDestroyed = false;
#define internalData ObjectFactoryData::get()

DEFINE_SINGLETON(ObjectFactoryData)
DEFINE_SINGLETON(ObjectFactory)

void ObjectFactoryData::release()
{
}

ObjectFactory::ObjectFactory()
{
    ObjectFactoryData::create();
}

void ObjectFactory::release()
{
    // Destroy all objects
    ObjectFactoryData::isDestroyed = true;
    for (const auto& object : internalData->objects)
    {
        if (object.sharedMemory)
        {
            internalData->releaseList.enqueue(object);
        }
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
        if (object->state == ObjectState::not_initialized)
        {
            object->init();
            object->state = ObjectState::initialized;

            // Add space in the objects pool
            if (internalData->freeIndex.empty())
            {
                internalData->extends();
            }

            // Insert the object
            object.sharedMemory->index = internalData->freeIndex.front();
            internalData->freeIndex.pop_front();
            internalData->objects[object.sharedMemory->index] = object;
        }
    }

    // Release objects
    std::vector<std::pair<HandleSharedMemory*, Object*>> toDelete;
    while (internalData->releaseList.try_dequeue(object))
    {
        if (object->state == ObjectState::initialized)
        {
            object->release();
            object->state = ObjectState::released;

            // Unallocate
            toDelete.push_back(std::make_pair(object.sharedMemory, object.data));

            // Remove from object pool
            uint64_t index = object.sharedMemory->index;
            internalData->objects[index] = nullptr;
            internalData->freeIndex.push_back(index);
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
