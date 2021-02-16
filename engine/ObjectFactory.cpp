#include "ObjectFactory.h"
#include "Handle.h"
#include "Object.h"
#include "ObjectFactoryData.h"
#include "utils/Logs.h"
#include <algorithm>

using namespace coral;

std::vector<Handle<Object>> ObjectFactoryData::objects;
std::vector<Handle<Object>> ObjectFactoryData::initializeList;
std::vector<Handle<Object>> ObjectFactoryData::releaseList;

DEFINE_SINGLETON(ObjectFactory)

void ObjectFactory::release()
{
    // Destroy all objects
    for (auto object : ObjectFactoryData::objects)
    {
        ObjectFactoryData::releaseList.push_back(object);
    }
    ObjectFactoryData::objects.clear();
    update();
}

void ObjectFactory::add(Handle<Object> object)
{
    ObjectFactoryData::objects.push_back(object);
    ObjectFactoryData::initializeList.push_back(object);
}

void ObjectFactory::remove(Handle<Object> object)
{
    // Move to release list
    ObjectFactoryData::releaseList.push_back(object);

    // Erase from object list
    auto it = std::find(ObjectFactoryData::objects.begin(), ObjectFactoryData::objects.end(), object);
    if (it != ObjectFactoryData::objects.end())
    {
        ObjectFactoryData::objects.erase(it);
    }
}

void ObjectFactory::update()
{
    // Initilize objects
    for (auto object : ObjectFactoryData::initializeList)
    {
        if (object->state == Object::ObjectState::not_initialized)
        {
            object->init();
            object->state = Object::ObjectState::initialized;
        }
    }

    // Release objects
    std::vector<std::pair<HandleSharedMemory*, Object*>> toDelete;
    for (auto object : ObjectFactoryData::releaseList)
    {
        if (object->state == Object::ObjectState::initialized)
        {
            object->release();
            object->state = Object::ObjectState::released;

            // Unallocate
            toDelete.push_back(std::make_pair(object.sharedMemory, object.data));
        }
    }

    ObjectFactoryData::initializeList.clear();
    ObjectFactoryData::releaseList.clear();

    // Deallocate
    for (const auto& pair : toDelete)
    {
        delete pair.first;
        delete pair.second;
    }
}