#pragma once
#include "utils/concurrentqueue.h"
#include <memory>
#include <vector>

namespace coral
{
    class Object;

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
        void add(const std::shared_ptr<Object>& object);
        void remove(const std::shared_ptr<Object>& object);
        void reset(const std::shared_ptr<Object>& object);

    private:
        // Init/release queues, thread safes
        moodycamel::ConcurrentQueue<std::shared_ptr<Object>> initializeList;
        moodycamel::ConcurrentQueue<std::shared_ptr<Object>> releaseList;
        moodycamel::ConcurrentQueue<std::shared_ptr<Object>> resetList;

        // Array with all the objects
        std::vector<std::shared_ptr<Object>> objects;
        std::vector<size_t> freePositions;
    };
}
