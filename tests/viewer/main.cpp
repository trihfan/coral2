#include "Object.h"

int main()
{
	auto object = std::make_shared<coral::Object>();
	object->setName("test");
	object = nullptr;
	//auto ttt = coral::ObjectInstance::objects;
	int t = 0;
}