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
        Logs(warning) << "object " << getName() << " has not been released";
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

Object::ObjectState Object::getState() const
{
    return state;
}

//*********************************************************************************
// ObjectManager

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
        object->state = Object::ObjectState::released;
    }

    objects.clear();
    initialize_list.clear();
    release_list.clear();
}


void ObjectManager::destroy(std::shared_ptr<Object> object)
{
    instance->unregisterObject(object);
}

ObjectManager::ObjectManager(std::shared_ptr<std::pmr::memory_resource> memory_resource) :
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
        if (object->state == Object::ObjectState::not_initialized)
        {
            object->init();
            object->state = Object::ObjectState::initialized;
        }
    }

    for (auto object : release_list)
    {
        if (object->state == Object::ObjectState::initialized)
        {
            object->release();
            object->state = Object::ObjectState::released;
        }
    }

    initialize_list.clear();
    release_list.clear();
}