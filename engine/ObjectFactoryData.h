#ifndef OBJECTFACTORYDATA_H
#define OBJECTFACTORYDATA_H

#include "utils/Singleton.h"
#include "utils/concurrentqueue.h"
#include <list>
#include <numeric>
#include <thread>
#include <vector>

namespace coral
{
    class Object;

    template <typename ObjectType>
    class Handle;

    /**
     * @brief Data for the Object factory
     */
    struct ObjectFactoryData
    {
        MAKE_SINGLETON(ObjectFactoryData)

    public:
        static ObjectFactoryData* get() { return instance.get(); }

        // Object pool, todo data structure
        std::vector<Handle<Object>> objects;
        std::list<uint32_t> freeIndex;

        // Initialize and release queues
        moodycamel::ConcurrentQueue<Handle<Object>> initializeList;
        moodycamel::ConcurrentQueue<Handle<Object>> releaseList;

        static bool isDestroyed;

        // Construction
        ObjectFactoryData()
        {
            extends();
        }

        void extends()
        {
            size_t start = objects.size();
            objects.resize(start + 1000);
            for (uint32_t i = static_cast<uint32_t>(start); i < objects.size(); i++)
            {
                freeIndex.push_back(i);
            }
        }
    };
}
#endif
