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

 namespace coral
{
    // The Object class represent an object in coral engine
    class Object : public std::enable_shared_from_this<Object>
    {
        friend class ObjectManager;
    public:
        // name
        void setName(const std::string& name);
        const std::string& getName() const;

        // handle
        template <typename ObjectType>
        std::shared_ptr<ObjectType> getHandle() const { return std::shared_ptr<ObjectType>(this); }

    protected:
        // construction
        Object();
        virtual ~Object();

        // initialization
        virtual void init() {}
        virtual void release() {}

    private:
        enum class ObjectState { not_initialized, initialized, destroyed } state;
        std::string name;
    };

    // The object manager
    class ObjectManager
    {
        MAKE_ENGINE_SINGLETON(ObjectManager)
    public:
        // Create an object of the given object type
        template <typename ObjectType, class... Args>
        static std::shared_ptr<ObjectType> create(Args... args);

        // Destroy the object, this should be called to released acquired data
        static void destroy(std::shared_ptr<Object> object);

    private:
        ObjectManager(std::pmr::memory_resource* memory_resource);
        void registerObject(std::shared_ptr<Object> object);
        void unregisterObject(std::shared_ptr<Object> object);

    private:
        // memory pool
        std::pmr::memory_resource* memory_resource;

        // object lists
        std::vector<std::shared_ptr<Object>> objects;
        std::vector<std::shared_ptr<Object>> initialize_list;
        std::vector<std::shared_ptr<Object>> release_list;
    };

    template <typename ObjectType, class... Args>
    std::shared_ptr<ObjectType> ObjectManager::create(Args... args)
    {
        static_assert (std::is_base_of<Object, ObjectType>::value, "Only object derivated from Object can be instanciated");

        // allocate object in the memory resource
        std::shared_ptr<ObjectType> object = std::allocate_shared(std::pmr::polymorphic_allocator<ObjectType>(instance->memory_resource), std::forward(args...));

        // register object
        instance->registerObject(object);

        // return created object
        return object;
    }
}
#endif
