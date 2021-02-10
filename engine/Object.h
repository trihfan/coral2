#ifndef OBJECT_H
#define OBJECT_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif

#include <memory>
#include <string>
#include <vector>
#include <type_traits>
#include "utils/Singleton.h"
#include "Signal.h"

namespace coral
{
    class Engine;

    // The Object class represent an object in coral engine
    class Object : public std::enable_shared_from_this<Object>
    {
        friend class ObjectManager;
    public:
        // type
        template<typename Type>
        bool isA() const { return dynamic_cast<const Type*>(this) != nullptr; }

        // name
        void setName(const std::string& name);
        const std::string& getName() const;

        // handle
        template <typename ObjectType>
        inline std::shared_ptr<ObjectType> toHandle() { return std::dynamic_pointer_cast<ObjectType>(shared_from_this()); }

        template <typename ObjectType>
        inline std::shared_ptr<const ObjectType> toHandle() const { return std::dynamic_pointer_cast<const ObjectType>(shared_from_this()); }

        // Signals
        Signal<> init;
        Signal<> release;
        Signal<> update;

    protected:
        // construction
        Object();
        virtual ~Object();

        // state
        enum class ObjectState { not_initialized, initialized, released } state;
        ObjectState getState() const;

    private:
        std::string name;
    };

    // The object manager
    class ObjectManager
    {
        MAKE_ENGINE_SINGLETON(ObjectManager)
    public:
        // Create an object of the given object type
        template <typename ObjectType, class... Args>
        static std::shared_ptr<ObjectType> createWithName(const std::string& name, Args&&... args);

        // Create an object of the given object type
        template <typename ObjectType, class... Args>
        static std::shared_ptr<ObjectType> create(Args&&... args);

        // Destroy the object, this should be called to released acquired data
        static void destroy(std::shared_ptr<Object> object);

    private:
        ObjectManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);
        void registerObject(std::shared_ptr<Object> object);
        void unregisterObject(std::shared_ptr<Object> object);

        // update all objects
        void update();

    private:
        // memory pool
        std::shared_ptr<std::pmr::memory_resource> memory_resource;

        // object lists
        std::vector<std::shared_ptr<Object>> objects;
        std::vector<std::shared_ptr<Object>> initialize_list;
        std::vector<std::shared_ptr<Object>> release_list;
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
        static_assert (std::is_base_of<Object, ObjectType>::value, "Only object derivated from Object can be instanciated");

        // allocate object in the memory resource
        std::shared_ptr<ObjectType> object = std::allocate_shared<ObjectType>(std::pmr::polymorphic_allocator<ObjectType>(instance->memory_resource.get()), std::forward<Args>(args)...);

        // register object
        instance->registerObject(object);

        // return created object
        return object;
    }
}
#endif
