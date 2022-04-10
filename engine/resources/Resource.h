#pragma once
#include "vulkan/VulkanBackendStructures.h"
#include "Object.h"
#include <memory>
#include <vector>

namespace coral
{
    enum class ResourceType
    {
        texture2d
    };

    // Texture internal format
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
    class Resource : public ObjectInterface
    {
    public:
        //
        Resource();

        // Properties
        Property<ResourceParams> params;

        // Load resource data from file
        bool load(const std::string& file);

        // Load resource data from memory
        bool load(const unsigned char* buffer, int length);

        // Init
        virtual void init() override;
        virtual void release() override;

        //
        const VulkanImage& vkHandle() const;
    private:
        // Vulkan data
        VulkanDevice device;
        VulkanImage image;
        bool ownImage;

        // Temporary ptr to resource data
        unsigned char* fileData;

        // Fill params from loaded data
        bool updateParams(int width, int height, int nbComponents);
    };
}
