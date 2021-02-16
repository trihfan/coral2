#ifndef OBJECTFACTORYDATA_H
#define OBJECTFACTORYDATA_H

#include "utils/Singleton.h"
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
        MAKE_SINGLETON(ObjectFactoryData)

    public:
        static ObjectFactoryData* get() { return instance.get(); }

        // Object pool, todo data structure
        std::vector<Handle<Object>> objects;

        // Initialize and release queues
        moodycamel::ConcurrentQueue<Handle<Object>> initializeList;
        moodycamel::ConcurrentQueue<Handle<Object>> releaseList;

        static bool isDestroyed;

        // Construction
        ObjectFactoryData()
            : initializeList(1000)
            , releaseList(1000)
        {
            objects.reserve(1000);
        }
    };
}
#endif
