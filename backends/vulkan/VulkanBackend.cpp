#include "VulkanBackend.h"
#include "CoralException.h"
#include "Logs.h"
#include "VulkanBackbuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanError.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanResource.h"
#include "VulkanValidation.h"
#include "VulkanVertexBuffer.h"
#include <algorithm>
#include <cstring>
#include <set>

using namespace coral;
using namespace backend::vulkan;

VulkanBackend* VulkanBackend::currentBackend = nullptr;

VulkanBackend* VulkanBackend::getCurrent()
{
    return currentBackend;
}

VulkanBackend::VulkanBackend(GLFWwindow* window)
    : window(window)
    , swapchain(nullptr)
{
    currentBackend = this;
}

VulkanBackend::~VulkanBackend()
{
    currentBackend = nullptr;
}

bool VulkanBackend::internalInit()
{
    try
    {
        createInstance();
        createSurface();
        getPhysicalDevice();
        createLogicalDevice();
        createSwapchain();
        createFrameSynchronization();

        Logs(success) << "Engine initialized";
    }
    catch (...)
    {
        return false;
    }

    // Set creators
    creator<BackendRenderPass, BackendRenderPassParams> = [this](const BackendRenderPassParams& params) { return std::make_unique<VulkanRenderPass>(params, mainDevice, swapchainFormat); };
    creator<BackendPipeline, BackendPipelineParams> = [this](const BackendPipelineParams& params) { return std::make_unique<VulkanPipeline>(params, mainDevice, swapchainExtent); };
    creator<BackendFramebuffer, BackendFramebufferCreationParams> = [this](const BackendFramebufferCreationParams& params) { return std::make_unique<VulkanFramebuffer>(params, mainDevice, swapchainExtent); };
    creator<BackendResource, BackendResourceParams> = [this](const BackendResourceParams& params) { return std::make_unique<VulkanResource>(params, mainDevice); };
    creator<BackendVertexBuffer, BackendVertexBufferData> = [this](const BackendVertexBufferData& data) { return std::make_unique<VulkanVertexBuffer>(data, mainDevice, graphicsQueue); };

    return true;
}

bool VulkanBackend::resize(int, int)
{
    deleteFrameSynchronization();
    deleteSwapchain();
    createSwapchain();
    createFrameSynchronization();
    return true;
}

void VulkanBackend::beginFrame()
{
    // -- GET NEXT IMAGE --
    // Wait for given fence to signal (open) from last draw before continuing
    vkWaitForFences(mainDevice.logicalDevice, 1, &drawFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    // Manually reset (close) fences
    vkResetFences(mainDevice.logicalDevice, 1, &drawFences[currentFrame]);

    // Get index of next image to be drawn to, and signal semaphore when ready to be drawn to
    vkAcquireNextImageKHR(mainDevice.logicalDevice, swapchain, std::numeric_limits<uint64_t>::max(), imageAvailable[currentFrame], VK_NULL_HANDLE, &imageIndex);

    VulkanCommandBufferManager::get()->begin();
}

void VulkanBackend::endFrame()
{
    VulkanCommandBufferManager::get()->end();

    // -- SUBMIT COMMAND BUFFER TO RENDER --
    // Queue submission information
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1; // Number of semaphores to wait on
    submitInfo.pWaitSemaphores = &imageAvailable[CURRENT_FRAME_INDEX]; // List of semaphores to wait on
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submitInfo.pWaitDstStageMask = waitStages; // Stages to check semaphores at
    submitInfo.commandBufferCount = 1; // Number of command buffers to submit
    submitInfo.pCommandBuffers = &VulkanCommandBufferManager::get()->getCommandBuffer(); // Command buffer to submit
    submitInfo.signalSemaphoreCount = 1; // Number of semaphores to signal
    submitInfo.pSignalSemaphores = &renderFinished[CURRENT_FRAME_INDEX]; // Semaphores to signal when command buffer finishes

    // Submit command buffer to queue
    VkResult result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[CURRENT_FRAME_INDEX]);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit Command Buffer to Queue!");
    }

    // -- PRESENT RENDERED IMAGE TO SCREEN --
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1; // Number of semaphores to wait on
    presentInfo.pWaitSemaphores = &renderFinished[currentFrame]; // Semaphores to wait on
    presentInfo.swapchainCount = 1; // Number of swapchains to present to
    presentInfo.pSwapchains = &swapchain; // Swapchains to present images to
    presentInfo.pImageIndices = &imageIndex; // Index of images in swapchains to present

    // Present image
    if (!VERIFY(vkQueuePresentKHR(presentationQueue, &presentInfo)))
    {
        Logs(error) << "Failed to present Image!";
    }

    // Get next frame (use % MAX_FRAME_DRAWS to keep value below MAX_FRAME_DRAWS)
    currentFrame = (currentFrame + 1) % swapChainImageCount;
}

bool VulkanBackend::internalRelease()
{
    // Wait until no actions being run on device before destroying
    vkDeviceWaitIdle(mainDevice.logicalDevice);

    deleteFrameSynchronization();
    deleteSwapchain();

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(mainDevice.logicalDevice, nullptr);
    vkDestroyInstance(instance, nullptr);
    return true;
}

backend::BackendCapabilities VulkanBackend::capabilities() const
{
    BackendCapabilities capabilities;
    capabilities.multithreadCapable = true;
    capabilities.glslVersion = 450;
    return capabilities;
}

void VulkanBackend::createInstance()
{
    // Information about the application
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Coral Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Information to create the instance
    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Create list to hold instance extensions
    std::vector<const char*> instanceExtensions;

    // Set up extensions Instance will use
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    // Get GLFW extensions
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Add GLFW extensions to list of extensions
    for (uint32_t i = 0; i < glfwExtensionCount; i++)
    {
        instanceExtensions.push_back(glfwExtensions[i]);
    }

    // Check Instance Extensions supported...
    if (!checkInstanceExtensionSupport(&instanceExtensions))
    {
        throw CoralException("VkInstance does not support required extensions!");
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    // TODO: Set up Validation Layers that Instance will use
#ifdef USE_VALIDATION_LAYERS
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    // Create instance
    if (!VERIFY(vkCreateInstance(&createInfo, nullptr, &instance)))
    {
        throw CoralException("Failed to create a Vulkan Instance!");
    }
}

void VulkanBackend::createSwapchain()
{
    SwapchainDetails swapchainDetails = getSwapChainDetails(mainDevice.physicalDevice);

    // Choose optimal surface format
    VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapchainDetails.formats);

    // Choose optimal presentation mode
    VkPresentModeKHR presentMode = chooseBestPresentationMode(swapchainDetails.presentationModes);

    // Choose swap chain resolution
    VkExtent2D extent = chooseSwapExtend(swapchainDetails.surfaceCapabilities);

    // Number of images in the swap chain, min image + 1 for being able to do triple buffering
    uint32_t imageCount = swapchainDetails.surfaceCapabilities.minImageCount + 1;
    if (swapchainDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapchainDetails.surfaceCapabilities.maxImageCount)
    {
        imageCount = swapchainDetails.surfaceCapabilities.maxImageCount;
    }

    // Fill create info
    VkSwapchainCreateInfoKHR swapchainCreateInfo {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.imageExtent = extent;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.preTransform = swapchainDetails.surfaceCapabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.clipped = VK_TRUE;

    // Get queue family indices
    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

    // If graphics && presentation family are different, then swapchain image must be shared between queue families
    if (indices.graphicsFamily != indices.presentationFamily)
    {
        uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.graphicsFamily) };

        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapchainCreateInfo.oldSwapchain = nullptr; // todo swap chain when resizing

    // Create swap chain
    if (!VERIFY(vkCreateSwapchainKHR(mainDevice.logicalDevice, &swapchainCreateInfo, nullptr, &swapchain)))
    {
        throw CoralException("Failed to create the swapchain!");
    }

    // Store swapchain details
    swapchainFormat = surfaceFormat.format;
    swapchainExtent = extent;

    // Get swap chain images
    uint32_t swapchainImageCount;
    vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapchainImageCount, nullptr);
    std::vector<VkImage> images(swapchainImageCount);
    vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapchainImageCount, images.data());

    // Create image views
    std::vector<std::unique_ptr<VulkanResource>> swapchainImages(images.size());
    for (size_t i = 0; i < images.size(); i++)
    {
        VulkanImage image { images[i], createImageView(images[i], swapchainFormat, VK_IMAGE_ASPECT_COLOR_BIT) };
        swapchainImages[i] = std::make_unique<VulkanResource>(image, mainDevice);
    }

    swapChainImageCount = swapchainImages.size();
    VulkanBackbuffer::setSwapChainImages(std::move(swapchainImages));
    VulkanCommandBufferManager::create(mainDevice, swapChainImageCount);
}

void VulkanBackend::getPhysicalDevice()
{
    // Enumerate Physical devices the vkInstance can access
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // If no devices available, then none support Vulkan!
    if (deviceCount == 0)
    {
        throw CoralException("Can't find GPUs that support Vulkan Instance!");
    }

    // Get list of Physical Devices
    std::vector<VkPhysicalDevice> deviceList(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

    for (const auto& device : deviceList)
    {
        if (checkDeviceSuitable(device))
        {
            mainDevice.physicalDevice = device;
            break;
        }
    }

    /*if (!mainDevice.physicalDevice)
    {
        throw
    }*/
}

void VulkanBackend::createLogicalDevice()
{
    // Get the queue family indices for the chosen Physical Device
    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> queueFamilyIndices { indices.graphicsFamily, indices.presentationFamily };

    for (int queueFamilyIndex : queueFamilyIndices)
    {
        VkDeviceQueueCreateInfo queueCreateInfo {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndex); // The index of the family to create a queue from
        queueCreateInfo.queueCount = 1; // Number of queues to create
        float priority = 1.0f;
        queueCreateInfo.pQueuePriorities = &priority; // Vulkan needs to know how to h&&le multiple queues, so decide priority (1 = highest priority)
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Information to create logical device (sometimes called "device")
    VkDeviceCreateInfo deviceCreateInfo {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()); // Number of Queue Create Infos
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data(); // List of queue create infos so device can create required queues
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()); // Number of enabled logical device extensions
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data(); // List of enabled logical device extensions

    // Physical Device Features the Logical Device will be using
    VkPhysicalDeviceFeatures deviceFeatures {};

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures; // Physical Device features Logical Device will use

    // Create the logical device for the given physical device
    if (!VERIFY(vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice)))
    {
        throw CoralException("Failed to create a Logical Device!");
    }

    // Queues are created at the same time as the device...
    // So we want h&&le to queues
    // From given logical device, of given Queue Family, of given Queue Index (0 since only one queue), place reference in given VkQueue
    vkGetDeviceQueue(mainDevice.logicalDevice, static_cast<uint32_t>(indices.graphicsFamily), 0, &graphicsQueue);
    vkGetDeviceQueue(mainDevice.logicalDevice, static_cast<uint32_t>(indices.presentationFamily), 0, &presentationQueue);
}

void VulkanBackend::createSurface()
{
    if (!VERIFY(glfwCreateWindowSurface(instance, window, nullptr, &surface)))
    {
        throw CoralException("Failed to create a surface!");
    }
}

bool VulkanBackend::checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
    // Need to get number of extensions to create array of correct size to hold extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (extensionCount == 0)
    {
        return false;
    }

    // Create a list of VkExtensionProperties using count
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    // Check if given extensions are in list of available extensions
    for (const auto& checkExtension : *checkExtensions)
    {
        bool hasExtension = false;
        for (const auto& extension : extensions)
        {
            if (strcmp(checkExtension, extension.extensionName))
            {
                hasExtension = true;
                break;
            }
        }

        if (!hasExtension)
        {
            return false;
        }
    }

    return true;
}

bool VulkanBackend::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    // Need to get number of extensions to create array of correct size to hold extensions
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    if (extensionCount == 0)
    {
        return false;
    }

    // Create a list of VkExtensionProperties using count
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

    // Check if given extensions are in list of available extensions
    for (const auto& checkExtension : deviceExtensions)
    {
        bool hasExtension = false;
        for (const auto& extension : extensions)
        {
            if (strcmp(checkExtension, extension.extensionName))
            {
                hasExtension = true;
                break;
            }
        }

        if (!hasExtension)
        {
            return false;
        }
    }

    return true;
}

bool VulkanBackend::checkDeviceSuitable(VkPhysicalDevice device)
{
    // Information about the device itself (ID, name, type, vendor, etc)
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    // Information about what the device can do (geo shader, tess shader, wide lines, etc)
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Get queue indices
    QueueFamilyIndices indices = getQueueFamilies(device);

    // Check device extensions
    bool deviceExtensionSupport = checkDeviceExtensionSupport(device);

    // Get swap chain details
    SwapchainDetails swapChainDetails = getSwapChainDetails(device);
    bool swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

    // Check validity
    if (deviceExtensionSupport && indices.isValid() && swapChainValid)
    {
        Logs(info) << deviceProperties.deviceName << " selected";
        return true;
    }
    return false;
}

QueueFamilyIndices VulkanBackend::getQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    // Get all Queue Family Property info for the given device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    // Go through each queue family && check if it has at least 1 of the required types of queue
    int i = 0;
    for (const auto& queueFamily : queueFamilyList)
    {
        // First check if queue family has at least 1 queue in that family (could have no queues)
        // Queue can be multiple types defined through bitfield. Need to bitwise && with VK_QUEUE_*_BIT to check if has required type
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i; // If queue family is valid, then get index
        }

        // Check if queue family support presentation
        VkBool32 presentationSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), surface, &presentationSupport);
        if (queueFamily.queueCount > 0 && presentationSupport)
        {
            indices.presentationFamily = i;
        }

        // Check if queue family indices are in a valid state, stop searching if so
        if (indices.isValid())
        {
            break;
        }

        i++;
    }

    return indices;
}

SwapchainDetails VulkanBackend::getSwapChainDetails(VkPhysicalDevice device)
{
    SwapchainDetails swapChaindetails;

    // Capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChaindetails.surfaceCapabilities);

    // Formats
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0)
    {
        swapChaindetails.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChaindetails.formats.data());
    }

    // Presentation modes
    uint32_t presentationModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationModeCount, nullptr);
    if (formatCount != 0)
    {
        swapChaindetails.presentationModes.resize(formatCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationModeCount, swapChaindetails.presentationModes.data());
    }

    return swapChaindetails;
}

VkSurfaceFormatKHR VulkanBackend::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
    // All formats available
    if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return VkSurfaceFormatKHR { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    // Search for optimal format
    for (const auto& format : formats)
    {
        if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }

    // Return by default
    Logs(warning) << "Can't choose best format";
    return formats[0];
}

VkPresentModeKHR VulkanBackend::chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes)
{
    // Search for optimal presentation mode
    for (const auto& presentationMode : presentationModes)
    {
        if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return presentationMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanBackend::chooseSwapExtend(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
    // Check if the current extend is defined
    if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return surfaceCapabilities.currentExtent;
    }
    // Otherwise compute from the window size
    else
    {
        // Get window size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Clamp extend to surface capabilities
        VkExtent2D extend;
        extend.width = std::clamp(static_cast<uint32_t>(width), surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        extend.height = std::clamp(static_cast<uint32_t>(height), surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

        return extend;
    }
}

VkImageView VulkanBackend::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    // Fill create info
    VkImageViewCreateInfo imageViewCreateInfo {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.image = image;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = format;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    // Subresources allow the view to view only a part of an image
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags; // Wich aspect of image to view (color, depth, ...)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    // Create the image view
    VkImageView imageView;
    if (!VERIFY(vkCreateImageView(mainDevice.logicalDevice, &imageViewCreateInfo, nullptr, &imageView)))
    {
        throw CoralException("Failed to create the image view!");
    }

    return imageView;
}

std::string VulkanBackend::getName() const
{
    return "vulkan";
}

QueueFamilyIndices VulkanBackend::getQueueFamilies()
{
    QueueFamilyIndices indices;

    // Get all Queue Family Property info for the given device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mainDevice.physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mainDevice.physicalDevice, &queueFamilyCount, queueFamilyList.data());

    // Go through each queue family and check if it has at least 1 of the required types of queue
    int i = 0;
    for (const auto& queueFamily : queueFamilyList)
    {
        // First check if queue family has at least 1 queue in that family (could have no queues)
        // Queue can be multiple types defined through bitfield. Need to bitwise AND with VK_QUEUE_*_BIT to check if has required type
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i; // If queue family is valid, then get index
        }

        // Check if Queue Family supports presentation
        VkBool32 presentationSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(mainDevice.physicalDevice, i, surface, &presentationSupport);
        // Check if queue is presentation type (can be both graphics and presentation)
        if (queueFamily.queueCount > 0 && presentationSupport)
        {
            indices.presentationFamily = i;
        }

        // Check if queue family indices are in a valid state, stop searching if so
        if (indices.isValid())
        {
            break;
        }

        i++;
    }

    return indices;
}

void VulkanBackend::createFrameSynchronization()
{
    currentFrame = 0;
    imageAvailable.resize(swapChainImageCount);
    renderFinished.resize(swapChainImageCount);
    drawFences.resize(swapChainImageCount);

    // Semaphore creation information
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    // Fence creation information
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < swapChainImageCount; i++)
    {
        if (!VERIFY(vkCreateSemaphore(mainDevice.logicalDevice, &semaphoreCreateInfo, nullptr, &imageAvailable[i])))
        {
            Logs(error) << "Failed to create image available semaphores";
        }

        if (!VERIFY(vkCreateSemaphore(mainDevice.logicalDevice, &semaphoreCreateInfo, nullptr, &renderFinished[i])))
        {
            Logs(error) << "Failed to create render finished semaphores";
        }

        if (!VERIFY(vkCreateFence(mainDevice.logicalDevice, &fenceCreateInfo, nullptr, &drawFences[i])))
        {
            Logs(error) << "Failed to create a draw fences";
        }
    }
}

void VulkanBackend::deleteSwapchain()
{
    // Clean
    VulkanCommandBufferManager::destroy();
    VulkanBackbuffer::release();
    if (swapchain)
    {
        vkDestroySwapchainKHR(mainDevice.logicalDevice, swapchain, nullptr);
    }
}

void VulkanBackend::deleteFrameSynchronization()
{
    for (size_t i = 0; i < swapChainImageCount; i++)
    {
        vkDestroySemaphore(mainDevice.logicalDevice, renderFinished[i], nullptr);
        vkDestroySemaphore(mainDevice.logicalDevice, imageAvailable[i], nullptr);
        vkDestroyFence(mainDevice.logicalDevice, drawFences[i], nullptr);
    }

    renderFinished.clear();
    imageAvailable.clear();
    drawFences.clear();
}

uint32_t VulkanBackend::getCurrentImageIndex() const
{
    return imageIndex;
}

size_t VulkanBackend::getCurrentFrameIndex() const
{
    return currentFrame;
}