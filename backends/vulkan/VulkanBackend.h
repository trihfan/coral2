#pragma once

#include "Backend.h"
#include "VulkanBackendStructures.h"

namespace backend::vulkan
{
    class VulkanBackend : public backend::Backend
    {
    public:
        VulkanBackend(GLFWwindow* window);

        // Overrided methods
        std::string getName() const override;
        bool resize(int width, int height) override;
        BackendCapabilities capabilities() const override;

    private:
        GLFWwindow* window;

        // Main vulkan Components
        VkInstance instance;
        VulkanDevice mainDevice;
        VkQueue graphicsQueue;
        VkQueue presentationQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapchain;
        std::vector<SwapchainImage> swapchainImages;

        // Utils vulkan components
        VkFormat swapchainFormat;
        VkExtent2D swapchainExtent;

        // Main functions
        bool internalInit() override;
        bool internalRelease() override;

        // Creation
        void createInstance();
        void createLogicalDevice();
        void createSurface();
        void createSwapchain();

        // Support functions
        // Get
        void getPhysicalDevice();
        QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
        SwapchainDetails getSwapChainDetails(VkPhysicalDevice device);

        // Check
        bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool checkDeviceSuitable(VkPhysicalDevice device);

        // Choose
        VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
        VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        // Create
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    };
}
