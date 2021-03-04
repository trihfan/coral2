#pragma once

#include <string>
#include <unordered_map>

namespace coral
{
    enum class ShaderType
    {
        vertex,
        fragment
    };

    struct Asset
    {
        std::string name;
        std::string url;
    };

    struct ShaderAsset
    {
        Asset asset;
        ShaderType type;
        std::string backend;
    };

    class AssetManager
    {
    public:
        static void init();

        //
        static ShaderAsset getShader(const std::string& name, ShaderType type);

    private:
        static std::unordered_map<std::string, std::vector<ShaderAsset>> shaders;
    };
}
