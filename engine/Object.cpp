#include "Object.h"

using namespace coral;


Object::Object()
{
	coral::ObjectInstance::registerObject(std::shared_ptr<Object>(this));
}

Object::~Object()
{
	if (!destroyed)
	{

	}
}

void Object::destroy()
{
	coral::ObjectInstance::unregisterObject(std::shared_ptr<Object>(this));
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

std::vector<std::shared_ptr<Object>> ObjectInstance::objects;
std::vector<std::shared_ptr<Object>> ObjectInstance::initialize_list;
std::vector<std::shared_ptr<Object>> ObjectInstance::release_list;

void ObjectInstance::registerObject(std::shared_ptr<Object> object)
{
	objects.push_back(object);
	initialize_list.push_back(object);
}

void ObjectInstance::unregisterObject(std::shared_ptr<Object> object)
{
	objects.erase(std::find(objects.begin(), objects.end(), object));
	release_list.push_back(object);
}