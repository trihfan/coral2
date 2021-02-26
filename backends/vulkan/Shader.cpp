#include "Shader.h"
#include "CoralException.h"
#include "Logs.h"
#include "VulkanError.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace backend::vulkan;

Shader::Shader(const std::string& filename, VkDevice device) :
    shaderModule(nullptr), device(device)
{
    // Load code
    std::vector<char> code = read(filename);

    // Shader Module creation information
    VkShaderModuleCreateInfo shaderModuleCreateInfo {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size(); // Size of code
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // Pointer to code (of uint32_t pointer type)

    if (!VERIFY(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule)))
    {
        throw coral::CoralException("Failed to create a shader module!");
    }
}

Shader::Shader(const Shader& other) :
    shaderModule(other.shaderModule), device(other.device)
{
}

Shader::~Shader()
{
    if (shaderModule)
    {
        vkDestroyShaderModule(device, shaderModule, nullptr);
    }
}

Shader::operator VkShaderModule() const
{
    return shaderModule;
}

std::vector<char> Shader::read(const std::string& filename) const
{
    // Open stream from given file
    // std::ios::binary tells stream to read file as binary
    // std::ios::ate tells stream to start reading from end of file
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // Check if file stream successfully opened
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open THE file!");
    }

    // Get current read position and use to resize file buffer
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> fileBuffer(fileSize);

    // Move read position (seek to) the start of the file
    file.seekg(0);

    // Read the file data into the buffer (stream "fileSize" in total)
    file.read(fileBuffer.data(), static_cast<std::streamsize>(fileSize));

    // Close stream
    file.close();

    return fileBuffer;
}

Shader Shader::fromGlsl(const std::string& filename, VkDevice device)
{
    // Convert
    std::string tempFile = std::filesystem::temp_directory_path().string() + "temp.spv";
    std::stringstream command;
    command << "glslangValidator -V \"" << filename << "\" -o \"" << tempFile << "\"";
    std::string commandStr = command.str();
    system(commandStr.c_str());

    // Load
    Shader shader(tempFile, device);

    // Clean up
    std::filesystem::remove(std::filesystem::path(tempFile));

    return shader;
}
