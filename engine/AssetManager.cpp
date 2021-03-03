#include "AssetManager.h"
#include "Logs.h"
#include "assets.h"
#include <vector>

using namespace coral;

std::unordered_map<std::string, Asset> AssetManager::assets;

void AssetManager::init()
{
    for (Asset asset : ASSETS_LIST)
    {
        //asset.url = asset.url;
        assets[asset.name] = asset;
    }
}

void AssetManager::addAsset(const std::string& name, const Asset& asset)
{
    assets[name] = asset;
}

Asset AssetManager::getAsset(const std::string& name)
{
    auto it = assets.find(name);
    if (it != assets.end())
    {
        return it->second;
    }

    Logs(error) << "asset " << name << " is not registred";
    return Asset();
}
