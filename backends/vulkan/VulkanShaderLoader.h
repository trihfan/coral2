#pragma once
#include "VulkanBackendStructures.h"
#include <array>

namespace backend::vulkan
{
    // supported shader types
    enum ShaderType : int
    {
        vertex,
        fragment
    };

    class VulkanShaderLoader
    {
    public:
        VulkanShaderLoader(VkDevice device);
        ~VulkanShaderLoader();

        void addShaderData(ShaderType type, const std::string& data);
        VkShaderModule getHandle(ShaderType type) const;
        void load();

    private:
        VkDevice device;
        std::array<VkShaderModule, 2> shaderModules;
        std::array<std::string, 2> shaderData;
        static const std::array<std::string, 2> shaderExtensions;

        VkShaderModule fromGlsl(const std::string& filename, VkDevice device) const;
    };
}
