#pragma once

#include <string>
#include <unordered_map>

namespace coral
{
    struct Asset
    {
        std::string name;
        std::string url;
    };

    class AssetManager
    {
    public:
        static void init();

        static void addAsset(const std::string& name, const Asset& asset);
        static Asset getAsset(const std::string& name);

    private:
        static std::unordered_map<std::string, Asset> assets;
    };
}
