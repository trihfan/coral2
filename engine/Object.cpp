#include <algorithm>
#include "Object.h"
#include "utils/Logs.h"

using namespace coral;

//*********************************************************************************
// Object

Object::Object() : state(ObjectState::not_initialized)
{
}

Object::~Object()
{
    if (state == ObjectState::initialized)
    {
        Logs(warning) << "object " << getName() << " has not been destroyed";
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

//*********************************************************************************
// ObjectFactory

std::unique_ptr<ObjectManager> ObjectManager::instance;

void ObjectManager::release()
{
    for (auto object : objects)
    {
        if (object->state == Object::ObjectState::initialized)
        {
            release_list.push_back(object);
        }
    }
    for (auto object : release_list)
    {
        object->release();
    }

    objects.clear();
    initialize_list.clear();
    release_list.clear();
}


void ObjectManager::destroy(std::shared_ptr<Object> object)
{
    instance->unregisterObject(object);
}

ObjectManager::ObjectManager(std::pmr::memory_resource* memory_resource) :
      memory_resource(memory_resource)
{
}

void ObjectManager::registerObject(std::shared_ptr<Object> object)
{
	objects.push_back(object);
	initialize_list.push_back(object);
}

void ObjectManager::unregisterObject(std::shared_ptr<Object> object)
{
	objects.erase(std::find(objects.begin(), objects.end(), object));
	release_list.push_back(object);
}

void ObjectManager::update()
{
    for (auto object : initialize_list)
    {
        object->release();
    }

    for (auto object : release_list)
    {
        object->release();
    }

    initialize_list.clear();
    release_list.clear();
}