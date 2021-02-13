#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "Object.h"
#include "memory_resource.h"
#include "utils/Singleton.h"
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace coral
{
    // The object manager
    class ObjectManager
    {
        MAKE_SINGLETON(ObjectManager)
    public:
        // Create an object of the given object type
        template <typename ObjectType, class... Args>
        static std::shared_ptr<ObjectType> createWithName(const std::string& name, Args&&... args);

        // Create an object of the given object type
        template <typename ObjectType, class... Args>
        static std::shared_ptr<ObjectType> create(Args&&... args);

        // Destroy the object, this should be called to released acquired data
        static void destroy(std::shared_ptr<Object> object);

        // update all objects
        static void update();

    private:
        ObjectManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);
        void registerObject(std::shared_ptr<Object> object);
        void unregisterObject(std::shared_ptr<Object> object);

    private:
        // memory pool
        std::shared_ptr<std::pmr::memory_resource> memory_resource;

        // object lists
        std::vector<std::shared_ptr<Object>> objects;
        std::vector<std::shared_ptr<Object>> initializeList;
        std::vector<std::shared_ptr<Object>> releaseList;
    };

    template <typename ObjectType, class... Args>
    std::shared_ptr<ObjectType> ObjectManager::createWithName(const std::string& name, Args&&... args)
    {
        std::shared_ptr<ObjectType> object = create<ObjectType>(std::forward<Args>(args)...);
        object->setName(name);
        return object;
    }

    template <typename ObjectType, class... Args>
    std::shared_ptr<ObjectType> ObjectManager::create(Args&&... args)
    {
        static_assert(std::is_base_of<Object, ObjectType>::value, "Only object derivated from Object can be instanciated");

        // allocate object in the memory resource
        std::shared_ptr<ObjectType> object = std::allocate_shared<ObjectType>(std::pmr::polymorphic_allocator<ObjectType>(instance->memory_resource.get()), std::forward<Args>(args)...);

        // register object
        instance->registerObject(object);

        // return created object
        return object;
    }
}
#endif
