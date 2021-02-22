#include "ResourceManager.h"
#include "FileUtils.h"
#include "Logs.h"

using namespace coral;

std::vector<std::filesystem::path> ResourceManager::paths;
std::unordered_map<std::string, ShaderFiles> ResourceManager::shaders;

void ResourceManager::addFolder(const std::filesystem::path& folder)
{
    paths.push_back(folder);
}

void ResourceManager::init()
{
    // list resources
    for (const std::filesystem::path& path : paths)
    {
        iterateFolder(path.is_relative() ? FileUtils::getAbsolutePath(path) : path);
    }
}

ShaderFiles ResourceManager::getShader(const std::string& name)
{
    auto it = shaders.find(name);
    if (it != shaders.end())
    {
        return it->second;
    }

    Logs(error) << "unknown shader " << name;
    return ShaderFiles();
}

void ResourceManager::iterateFolder(const std::filesystem::path& path)
{
    listShaders(path);

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry))
        {
            iterateFolder(entry.path());
        }
    }
}

// todo -> TO BACKEND
static bool isVertex(const std::string& extension) { return extension == ".vert" || extension == ".vs" || extension == ".vs.glsl"; }
static bool isFragment(const std::string& extension) { return extension == ".frag" || extension == ".fs" || extension == ".fs.glsl"; }

void ResourceManager::listShaders(const std::filesystem::path& path)
{
    // list shaders in folder
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::string extension = entry.path().extension();
            std::string name = entry.path().stem().string();

            // vertex
            if (isVertex(extension))
            {
                shaders[name].vertexFile = entry.path();
            }
            // fragment
            else if (isFragment(extension))
            {
                shaders[name].fragmentFile = entry.path();
            }
        }
    }
}
