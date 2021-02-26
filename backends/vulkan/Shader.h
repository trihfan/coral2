/**
 *
 */

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#pragma once

namespace backend::vulkan
{
    class Shader
    {
    public:
        /**
         * @brief Read and load from a glsl shader
         */
        static Shader fromGlsl(const std::string& filename, VkDevice device);

        /**
         * Constructor
         */
        Shader(const std::string& filename, VkDevice device);

        /**
         * @brief Copy constructor
         */
        Shader(const Shader& other);

        /**
         * Destructor
         */
        ~Shader();

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
