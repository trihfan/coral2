#ifndef OBJECTFACTORYDATA_H
#define OBJECTFACTORYDATA_H

#include "utils/concurrentqueue.h"
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
        // Object pool, todo data structure
        std::vector<Handle<Object>> objects;

        // Initialize and release queues
        moodycamel::ConcurrentQueue<Handle<Object>> initializeList;
        moodycamel::ConcurrentQueue<Handle<Object>> releaseList;

        static ObjectFactoryData* instance;
        ObjectFactoryData()
            : initializeList(1000)
            , releaseList(1000)
        {
            objects.reserve(1000);
        }
    };
}
#endif
