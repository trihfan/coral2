#pragma once
#include "utils/ConcurrentQueue.h"
#include "Handle.h"
#include <memory>
#include <string>
#include <vector>

namespace coral
{
    template <typename Type> class Object;
    class ObjectInterface;

    /**
     * @brief Mange the object, calling init and release
     */
    class ObjectManager
    {
    public:
        // Construction
        ObjectManager();
        ~ObjectManager();

        // Init and release objects
        void update();

        // Handle objects
        void add(const Object<ObjectInterface>& object);
        void remove(const Object<ObjectInterface>& object);
        void reset(const Object<ObjectInterface>& object);

    private:
        // Init/release queues, thread safes
        moodycamel::ConcurrentQueue<Object<ObjectInterface>> initializeList;
        moodycamel::ConcurrentQueue<Object<ObjectInterface>> releaseList;
        moodycamel::ConcurrentQueue<Object<ObjectInterface>> resetList;

        // Array with all the objects
        std::vector<Object<ObjectInterface>> objects;
        std::vector<size_t> freePositions;
    };
}
