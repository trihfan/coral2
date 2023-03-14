#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>
#include "VulkanError.h"

namespace coral
{
    struct VulkanImage
    {
        VkImage image;
        VkImageView imageView;
    };
}
