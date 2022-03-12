#pragma once

#include "VulkanBackendStructures.h"
#include "utils/Singleton.h"
#include <vector>

#define CURRENT_IMAGE_INDEX VulkanBackend::getCurrent()->getCurrentImageIndex()
#define CURRENT_FRAME_INDEX VulkanBackend::getCurrent()->getCurrentFrameIndex()

namespace coral
{
    class VulkanResource;

    class VulkanBackend : public Singleton<VulkanBackend>
    {
    public:
        // Overrided methods
        std::string getName() const override;
        bool resize(int width, int height) override;
        void beginFrame() override;
        void endFrame() override;
        BackendCapabilities capabilities() const override;

        QueueFamilyIndices getQueueFamilies();
        uint32_t getCurrentImageIndex() const;
        size_t getCurrentFrameIndex() const;

    private:
        GLFWwindow* window;
        size_t threadCount;

        // Main vulkan Components
        VkInstance instance;
        VulkanDevice mainDevice;
        VkQueue graphicsQueue;
        VkQueue presentationQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapchain;
        size_t swapChainImageCount;
        uint32_t imageIndex;

        // Frame synchronization
        size_t currentFrame;
        std::vector<VkSemaphore> imageAvailable;
        std::vector<VkSemaphore> renderFinished;
        std::vector<VkFence> drawFences;

        // Utils vulkan components
        VkFormat swapchainFormat;
        VkExtent2D swapchainExtent;

        // Constructor
        VulkanBackend(GLFWwindow* window);      

        // Main functions
        bool internalInit() override;
        bool internalRelease() override;

        // Creation
        void createInstance();
        void createLogicalDevice();
        void createSurface();
        void createSwapchain();
        void createFrameSynchronization();

        // Deletion
        void deleteSwapchain();
        void deleteFrameSynchronization();

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
