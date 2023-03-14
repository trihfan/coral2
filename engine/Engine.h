#pragma once
#include <chrono>
#include <vector>
#include <vulkan/vulkan.h>
#include "Config.h"

class GLFWwindow;

namespace coral
{
    class Config;
    class RenderParameters;
    class SceneManager;
    class ObjectManager;
    class RenderPassManager;
    class PipelineManager;
    class CommandBufferManager;
    class Freetype;
    class Engine;

    using Clock = std::chrono::high_resolution_clock;

    /********************************************************/
    inline static const std::vector<const char*> deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

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

    /********************************************************/

    /**
     * @brief Static container for the currently running engine
     */
    class CurrentEngine
    {
    public:
        // Creation
        void init(int width, int height) { engine = std::make_unique<Engine>(width, height); }
        void destroy() { engine = nullptr; }
        Engine* operator->() { return engine.get(); }

    private:
        static std::unique_ptr<Engine> engine;
    };

    // The engine to use
    inline static CurrentEngine engine;

    /**
     * @brief Main class of the engine
     */
    class Engine
    {
    public:
        // Construction
        Engine(int width, int height);
        ~Engine();

        // Resize the engine output
        void resize(int width, int height);

        // Draw a frame
        void frame();

        // Properties
        Config config;
        std::unique_ptr<RenderParameters> renderParameters;

        // Modules
        std::unique_ptr<SceneManager> sceneManager;
        std::unique_ptr<ObjectManager> objectManager;
        std::unique_ptr<RenderPassManager> renderPassManager;
        std::unique_ptr<PipelineManager> pipelineManager;
        std::unique_ptr<CommandBufferManager> commandBufferManager;
        std::unique_ptr<Freetype> freetype;

    private:
        // Time point of engine start
        Clock::time_point startTime;

        //
        GLFWwindow* window;

        // Main vulkan Components
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
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

        // Vulkan init
        void initVulkan();
        void releaseVulkan();

        // Handle frame
        void beginFrame();
        void endFrame();

        // -- Vulkan creation
        void createInstance();
        void createLogicalDevice();
        void createSurface();
        void createSwapchain();
        void createFrameSynchronization();

        // -- Vulkan deletion
        void deleteSwapchain();
        void deleteFrameSynchronization();

        // -- Vulkan support functions
        // Get
        void selectPhysicalDevice();
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
