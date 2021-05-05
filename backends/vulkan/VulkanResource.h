#pragma once

#include "BackendResource.h"
#include "VulkanBackendStructures.h"

namespace backend::vulkan
{
    class VulkanResource : public BackendResource
    {
    public:
        VulkanResource(const BackendResourceParams& params, const VulkanDevice& device);
        VulkanResource(const VulkanImage& image, const VulkanDevice& device);
        ~VulkanResource() override;
        virtual void bind(int index) override;

        const VulkanImage& getImage() const;

    private:
        VulkanDevice device;
        VulkanImage image;
        bool ownImage;
    };
}
