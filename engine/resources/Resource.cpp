#include "Resource.h"
#include "utils/Error.h"

using namespace coral;

//*********************************************************************************
// Resource

Resource::Resource() : id(0)
{
    connect<&Resource::init>(Object::init, this);
    connect<&Resource::release>(Object::release, this);
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
    glDeleteTextures(1, &id);
    CHECK_OPENGL_ERROR
}

//*********************************************************************************
// ResourceManager

DEFINE_SINGLETON(ResourceManager)

void ResourceManager::clear()
{
    instance->resourceByName.clear();
}

std::shared_ptr<Resource> ResourceManager::getResourceByName(const std::string& name)
{
    auto it = instance->resourceByName.find(name);
    if (it != instance->resourceByName.end())
    {
        return it->second->toHandle<Resource>();
    }

    return nullptr;
}

ResourceManager::ResourceManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void ResourceManager::registerResource(std::shared_ptr<Resource> resource)
{
    auto it = instance->resourceByName.find(resource->getName());
    if (it != instance->resourceByName.end())
    {
        Logs(error) << "Resource name is already used";
        return;
    }

    instance->resourceByName[resource->getName()] = resource;
}