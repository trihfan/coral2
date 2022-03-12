#pragma once

#include "vulkan/VulkanBackendStructures.h"
#include "base/Object.h"
#include <memory>

namespace coral
{
    class Engine;

    /**
     * @brief Texture type
     */
    enum class ResourceType
    {
        texture2d
    };

    /**
     * @brief Texture internal format
     */
    enum class ResourceFormat
    {
        r8,
        rg88,
        rgb888,
        rgba8888,
        argb8888,
        depth24_pencil8
    };

    struct ResourceParams
    {
        ResourceType type;
        ResourceFormat format;
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

        const VulkanImage& getImage() const;
        void bind(int index);

        virtual void init();
        virtual void release();

    private:
        ResourceParams params;
        unsigned char* fileData;
        
        VulkanDevice device;
        VulkanImage image;
        bool ownImage;

        void setFromMemory();
    };
}
