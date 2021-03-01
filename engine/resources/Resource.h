#pragma once

#include "BackendResource.h"
#include "Object.h"
#include "Property.h"
#include <memory>

namespace coral
{
    class Engine;

    /**
     * @brief The Resource class represent a gpu resource
     * It encapsulate the BackendResource
     */
    class Resource : public Object
    {
    public:
        Resource();
        void bind();
        backend::BackendResource* getBackendResource() const;

        // Properties
        Property<backend::BackendResourceParams> params;

    private:
        std::unique_ptr<backend::BackendResource> backendResource;

        // initialization
        void init();
        void release();
    };
}
