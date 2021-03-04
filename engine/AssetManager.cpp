#include "AssetManager.h"
#include "Backend.h"
#include "Logs.h"
//#include "assets.h"
#include <vector>

using namespace coral;

std::unordered_map<std::string, std::vector<ShaderAsset>> AssetManager::shaders;

void AssetManager::init()
{
    /*for (const ShaderAsset& shaderAsset : SHADER_ASSETS_LIST)
    {
        // Update relative url
        //asset.url = asset.url;

        shaders[shaderAsset.asset.name] = shaderAsset;
    }*/
}

ShaderAsset AssetManager::getShader(const std::string& name, ShaderType type)
{
    ShaderAsset asset;
    asset.backend = backend::Backend::current()->getName();
    asset.type = type;

    asset.asset.name = name;
    asset.asset.url = "assets/shaders/" + backend::Backend::current()->getName() + "/" + name;

    switch (type)
    {
    case ShaderType::vertex:
        asset.asset.url += ".vert";
        break;

    case ShaderType::fragment:
        asset.asset.url += ".frag";
        break;
    }

    return asset;
    /*auto it = shaders.find(name);
    if (it != shaders.end())
    {
        for (const ShaderAsset& shaderAsset : it->second)
        {
            if (shaderAsset.type == type && shaderAsset.backend == backend::Backend::currentBackend()->getname())
            {
                return shaderAsset;
            }
        }
    }

    Logs(error) << "shader asset " << name << " not found";
    return ShaderAsset();*/
}
