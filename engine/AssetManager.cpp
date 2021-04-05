#include "AssetManager.h"
#include "Backend.h"
#include "FileUtils.h"
#include "Logs.h"
#include <filesystem>
#include <vector>

using namespace coral;

std::unordered_map<std::string, Asset> AssetManager::assets;

void AssetManager::addDirectory(const std::string& directory)
{
#ifndef __EMSCRIPTEN__ // <- todo for emscripten
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            auto path = entry.path();

            // Add new asset
            Asset asset;
            asset.name = path.string();
            asset.name = asset.name.erase(asset.name.find(directory), directory.length() + 1);

            asset.url = path.string();
            asset.size = std::filesystem::file_size(entry);

            const std::string fullname = directory + ":" + asset.name;
            assets[fullname] = asset;
        }
    }
#endif
}

Asset AssetManager::get(const std::string& parent, const std::string& name)
{
#ifdef __EMSCRIPTEN__
    return Asset { name, parent + "/" + name, 0 };
#endif

    const std::string fullname = parent + ":" + name;

    auto it = assets.find(fullname);
    if (it != assets.end())
    {
        return it->second;
    }

    return Asset();
}