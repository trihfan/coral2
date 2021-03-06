#pragma once

#include "BackendResource.h"
#include "Object.h"
#include "Property.h"
#include <memory>

namespace coral
{
    class Engine;

    struct ResourceParams
    {
        backend::BackendResourceType type;
        backend::BackendResourceFormat format;
        int width;
        int height;
        int samples = 1;
        std::vector<unsigned char> data; // optional data
    };

    /**
     * @brief The Resource class represent a gpu resource
     * It encapsulate the BackendResource
     */
    class Resource : public Object
    {
    public:
        Resource(const ResourceParams& params);

        // Create resource from file
        Resource(const std::string& file);

        void bind(int index);
        backend::BackendResource* getBackendResource() const;

    private:
        ResourceParams params;
        unsigned char* fileData;
        std::unique_ptr<backend::BackendResource> backendResource;

        // initialization
        void init();
        void release();
    };
}
