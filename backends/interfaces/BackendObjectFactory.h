#pragma once
#include "Logs.h"
#include <functional>
#include <memory>

namespace backend
{
    template <typename Object, typename... Args>
    std::function<std::unique_ptr<Object>(const Args&...)> creator;

    /**
     * Generic factory used to create backend objects
     */
    template <typename Object>
    class BackendObjectFactory
    {
    public:
        /**
         * Create the object
         */
        template <typename... Args>
        static std::unique_ptr<Object> create(const Args&... args)
        {
            if (!creator<Object, Args...>)
            {
                coral::Logs(coral::error) << "null creator";
                return nullptr;
            }
            return creator<Object, Args...>(args...);
        }
    };
}
