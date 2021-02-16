#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "utils/Singleton.h"
#include <string>

namespace coral
{
    class Object;
    struct HandleSharedMemory;
    template <typename ObjectType>
    class Handle;

    /**
     * @brief Factory for all engine objects
     */
    class ObjectFactory
    {
        MAKE_SINGLETON(ObjectFactory)

    public:
        /**
         * @brief Create an object of the given object type
         */
        template <typename ObjectType, class... Args>
        static Handle<ObjectType> createWithName(const std::string& name, Args&&... args);

        /**
         * @brief Create an object of the given object type
         */
        template <typename ObjectType, class... Args>
        static Handle<ObjectType> create(Args&&... args);

        /**
         * @brief Update all objects
         */
        static void update();

    private:
        ObjectFactory() = default;

        /**
         * @brief Add an object to be managed by the factory
         */
        void add(Handle<Object> object);

        /**
         * @brief Remove the object (deallocate resources)
         */
        void remove(Handle<Object> object);
    };

    template <typename ObjectType, class... Args>
    Handle<ObjectType> ObjectFactory::createWithName(const std::string& name, Args&&... args)
    {
        Handle<ObjectType> object = create<ObjectType>(std::forward<Args>(args)...);
        object->setName(name);
        return object;
    }

    template <typename ObjectType, class... Args>
    Handle<ObjectType> ObjectFactory::create(Args&&... args)
    {
        static_assert(std::is_base_of<Object, ObjectType>::value, "Only object derivated from Object can be instanciated");

        // allocate object
        ObjectType* object = new ObjectType(std::forward<Args>(args)...);
        HandleSharedMemory* sharedMemory = new HandleSharedMemory();

        Handle<ObjectType> handle(object, sharedMemory);

        // register object
        instance->add(handle);

        // return created object
        return handle;
    }
}
#endif
