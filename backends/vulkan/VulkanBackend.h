#pragma once

#include "Backend.h"
#include "VulkanBackendStructures.h"

namespace coral
{
    class VulkanBackend : public Backend
    {
    public:
        VulkanBackend(GLFWwindow* window);

        bool init() override;
        bool destroy() override;
        bool resize(int width, int height) override;

        std::unique_ptr<BackendPipeline> createPipeline(const BackendPipelineParams& params) const override { return nullptr; }

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
