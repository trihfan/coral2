#pragma once

#include "BackendResource.h"
#include "base/Object.h"
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

        // Create resource from memory
        Resource(const unsigned char* buffer, int length);

        void bind(int index);
        backend::BackendResource* getBackendResource() const;

        virtual void init();
        virtual void release();

    private:
        ResourceParams params;
        unsigned char* fileData;
        std::unique_ptr<backend::BackendResource> backendResource;

        void setFromMemory();
    };
}
