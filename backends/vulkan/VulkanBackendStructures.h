#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace backend::vulkan
{
    const std::vector<const char*> deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    struct VulkanDevice
    {
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
    };

    struct QueueFamilyIndices
    {
        int graphicsFamily = -1; // Location of the graphics family
        int presentationFamily = -1; // Location of the presentation family

        // Check if queue families are valid
        bool isValid()
        {
            return graphicsFamily >= 0 && presentationFamily >= 0;
        }
    };

    struct SwapchainDetails
    {
        VkSurfaceCapabilitiesKHR surfaceCapabilities; // Surface properties
        std::vector<VkSurfaceFormatKHR> formats; // List of supported surface image formats
        std::vector<VkPresentModeKHR> presentationModes; // List of supported presentation modes
    };

    struct VulkanImage
    {
        VkImage image;
        VkImageView imageView;
    };
}
