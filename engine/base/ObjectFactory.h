#pragma once

#include "Ptr.h"
#include "EngineModule.h"
#include "utils/concurrentqueue.h"
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace coral
{
    class Object;

    /**
     * @brief Factory for all engine objects
     */
    class ObjectFactory : public EngineModule<ObjectFactory>
    {
    public:
        /**
         * @brief Create an object of the given object type
         */
        template <typename ObjectType, class... Args>
        static ptr<ObjectType> createWithName(const std::string& name, Args&&... args);

        /**
         * @brief Create an object of the given object type
         */
        template <typename ObjectType, class... Args>
        static ptr<ObjectType> create(Args&&... args);

        /**
         * @brief Initialize and release objects
         */
        void update();

    private:
        moodycamel::ConcurrentQueue<ptr<Object>> initializeList;
        moodycamel::ConcurrentQueue<ptr<Object>> releaseList;
        std::vector<ptr<Object>> objects;
        std::list<size_t> freePositions;

        // Constructor
        ObjectFactory();
    };

    template <typename ObjectType, class... Args>
    ptr<ObjectType> ObjectFactory::createWithName(const std::string& name, Args&&... args)
    {
        ptr<ObjectType> object = create<ObjectType>(std::forward<Args>(args)...);
        object->setName(name);
        return object;
    }

    template <typename ObjectType, class... Args>
    ptr<ObjectType> ObjectFactory::create(Args&&... args)
    {
        static_assert(std::is_base_of<Object, ObjectType>::value, "Only object derivated from Object can be instanciated");
        ptr<ObjectType> object = std::make_shared<ObjectType>(std::forward<Args>(args)...);
        initializeList.enqueue(object);
        return object;
    }
}
