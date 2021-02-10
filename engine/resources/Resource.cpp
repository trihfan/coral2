#include "Resource.h"
#include "utils/Error.h"

using namespace coral;

//*********************************************************************************
// Resource

Resource::Resource() : id(0)
{
}
  
void Resource::bind()
{
    glBindTexture(*sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
}

GLuint Resource::getId() const
{
    return id;
}

void Resource::init()
{
    glGenTextures(1, &id);
    glBindTexture(*sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
    glTexImage2D(*sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, 0, *internalFormat, *width, *height, 0, *format, *type, nullptr);
    CHECK_OPENGL_ERROR
}

void Resource::release()
{
    CHECK_OPENGL_ERROR
}

//*********************************************************************************
// ResourceManager

DEFINE_SINGLETON(ResourceManager)

void ResourceManager::clear()
{
    instance->resources.clear();
}

std::shared_ptr<Resource> ResourceManager::getResourceByName(const std::string& name)
{
    auto it = std::find_if(instance->resources.begin(), instance->resources.end(), [&name](const auto& resource)
    {
        return resource->getName() == name;
    });
    if (it != instance->resources.end())
    {
        return *it;
    }

    std::shared_ptr<Resource> resource = ObjectManager::createWithName<Resource>(name);
    instance->resources.push_back(resource);
    return resource;
}

ResourceManager::ResourceManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}