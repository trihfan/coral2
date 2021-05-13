#include "VulkanShaderLoader.h"
#include "FileUtils.h"
#include "Logs.h"
#include "VulkanError.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace backend::vulkan;
using namespace coral;

const std::array<std::string, 2> VulkanShaderLoader::shaderExtensions { ".vert", ".frag" };

VulkanShaderLoader::VulkanShaderLoader(VkDevice device)
    : device(device)
{
    for (size_t i = 0; i < shaderModules.size(); i++)
    {
        shaderModules[i] = VK_NULL_HANDLE;
    }
}

VulkanShaderLoader::~VulkanShaderLoader()
{
    for (size_t i = 0; i < shaderModules.size(); i++)
    {
        if (shaderModules[i] != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(device, shaderModules[i], nullptr);
        }
    }
}

void VulkanShaderLoader::addShaderData(ShaderType type, const std::string& data)
{
    std::stringstream head;
    head << "#version 450" << std::endl;
    head << data;
    shaderData[type] = head.str();
}

VkShaderModule VulkanShaderLoader::getHandle(ShaderType type) const
{
    return shaderModules[type];
}

void VulkanShaderLoader::load()
{
    for (size_t i = 0; i < shaderModules.size(); i++)
    {
        // Save temp file
        //std::string tempFile = std::filesystem::temp_directory_path().string() + "temp" + shaderExtensions[i];
        std::string tempFile = "/Users/tcardaire/Downloads/temp" + shaderExtensions[i];
        std::ofstream glslShaderFile(tempFile);
        if (!glslShaderFile.is_open())
        {
            Logs(error) << "Output temp file not open";
            continue;
        }
        glslShaderFile << shaderData[i];
        glslShaderFile.close();

        // Convert and load as vulkan shader
        shaderModules[i] = fromGlsl(tempFile, device);

        // Clean up
        //std::filesystem::remove(std::filesystem::path(tempFile));
    }
}

VkShaderModule VulkanShaderLoader::fromGlsl(const std::string& filename, VkDevice device) const
{
    // Convert
    std::string tempFile = std::filesystem::temp_directory_path().string() + "temp.spv";
    std::stringstream command;
    command << "glslangValidator -V " << filename << " -o " << tempFile;
    std::string commandStr = command.str();
    system(commandStr.c_str());

    // Load
    std::string code = FileUtils::readAll(tempFile);
    VkShaderModuleCreateInfo shaderModuleCreateInfo {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size(); // Size of code
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // Pointer to code (of uint32_t pointer type)

    VkShaderModule shaderModule;
    if (!VERIFY(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule)))
    {
        Logs(error) << "Failed to create a shader module!";
    }

    // Clean up
    std::filesystem::remove(std::filesystem::path(tempFile));
    return shaderModule;
}