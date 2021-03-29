#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

namespace coral
{
    struct Asset
    {
        std::string name;
        std::string url;
        uint64_t size;
    };

    class AssetManager
    {
    public:
        // Add assets to the manager
        static void addDirectory(const std::string& directory);
        //static void addDictionnary(const std::string& dictionnary);

        // Get an asset
        static Asset get(const std::string& parent, const std::string& name);

    private:
        static std::unordered_map<std::string, Asset> assets;
    };
}
