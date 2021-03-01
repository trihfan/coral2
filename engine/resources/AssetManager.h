#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace coral
{
    /**
     * @brief A shader files on the filesystem
     */
    struct ShaderFiles
    {
        std::string vertexFile;
        std::string fragmentFile;
    };

    /**
     * @brief The AssetManager class handle resources filepaths
     */
    class AssetManager
    {
    public:
        /**
         * @brief Add a folder to search for resources
         */
        static void addFolder(const std::filesystem::path& folder);

        /**
         * @brief Iterator all folder and list resources
         */
        static void init();

        // Shaders
        static ShaderFiles getShader(const std::string& name);

    private:
        // Folders
        static std::vector<std::filesystem::path> paths;
        static void iterateFolder(const std::filesystem::path& path);

        // Shaders
        static std::unordered_map<std::string, ShaderFiles> shaders;
        static void listShaders(const std::filesystem::path& path);
    };
}
