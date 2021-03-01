#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace backend::vulkan
{
    class VulkanShader
    {
    public:
        /**
         * @brief Read and load from a glsl shader
         */
        static VulkanShader fromGlsl(const std::string& filename, VkDevice device);

        /**
         * Constructor
         */
        VulkanShader(const std::string& filename, VkDevice device);

        /**
         * @brief Copy constructor
         */
        VulkanShader(const VulkanShader& other);

        /**
         * Destructor
         */
        ~VulkanShader();

        /**
         * Return the vulkan handle
         */
        operator VkShaderModule() const;

    private:
        /**
         * The shader module
         */
        VkShaderModule shaderModule;

        /**
         * @brief The device on wich create the shader
         */
        VkDevice device;

        /**
         * @brief read file content
         */
        std::vector<char> read(const std::string& filename) const;
    };
}
