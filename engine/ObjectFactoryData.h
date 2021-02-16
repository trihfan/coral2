#ifndef OBJECTFACTORYDATA_H
#define OBJECTFACTORYDATA_H

#include <vector>

namespace coral
{
    class Object;
    template <typename ObjectType>
    class Handle;

    class SafeList
    {
    public:
        void push_back();
    };

    /**
     * @brief Data for the Object factory
     */
    class ObjectFactoryData
    {
    public:
        static std::vector<Handle<Object>> objects;
        static std::vector<Handle<Object>> initializeList;
        static std::vector<Handle<Object>> releaseList;
    };
}
#endif
