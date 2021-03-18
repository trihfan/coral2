#include "Resource.h"
#include "BackendObjectFactory.h"
#include "stb_image.h"

using namespace coral;

Resource::Resource(const ResourceParams& params)
    : params(params)
    , fileData(nullptr)
{
}

Resource::Resource(const std::string& file)
{
    // Load file
    int width, height;
    int nbComponents;
    fileData = stbi_load(file.c_str(), &width, &height, &nbComponents, 0);

    if (!fileData)
    {
        Logs(error) << "Texture failed to load (null) at path: " << file;
        return;
    }

    switch (nbComponents)
    {
    case 1:
        params.format = backend::BackendResourceFormat::r8;
        break;

    case 2:
        params.format = backend::BackendResourceFormat::rg88;
        break;

    case 3:
        params.format = backend::BackendResourceFormat::rgb888;
        break;

    case 4:
        params.format = backend::BackendResourceFormat::rgba8888;
        break;

    default:
        Logs(error) << "Texture failed to load (wrong component size) at path: " << file;
        return;
    }

    params.type = backend::BackendResourceType::texture2d;
    params.width = width;
    params.height = height;
}

Resource::Resource(const unsigned char* buffer, int length)
{
    // Load file
    int width, height;
    int nbComponents;
    fileData = stbi_load_from_memory(buffer, length, &width, &height, &nbComponents, 0);

    if (!fileData)
    {
        Logs(error) << "Texture failed to load from memory";
        return;
    }

    switch (nbComponents)
    {
    case 1:
        params.format = backend::BackendResourceFormat::r8;
        break;

    case 2:
        params.format = backend::BackendResourceFormat::rg88;
        break;

    case 3:
        params.format = backend::BackendResourceFormat::rgb888;
        break;

    case 4:
        params.format = backend::BackendResourceFormat::rgba8888;
        break;

    default:
        Logs(error) << "Texture failed to load from memory";
        return;
    }

    params.type = backend::BackendResourceType::texture2d;
    params.width = width;
    params.height = height;
}

void Resource::bind(int index)
{
    backendResource->bind(index);
}

backend::BackendResource* Resource::getBackendResource() const
{
    return backendResource.get();
}

void Resource::init()
{
    Object::init();
    backend::BackendResourceParams params;
    params.type = this->params.type;
    params.format = this->params.format;
    params.width = this->params.width;
    params.height = this->params.height;
    params.samples = this->params.samples;
    params.data = this->params.data.empty() ? fileData : this->params.data.data();
    backendResource = backend::BackendObjectFactory<backend::BackendResource>::create(params);

    // Release memory
    this->params.data.clear();

    if (fileData)
    {
        stbi_image_free(fileData);
        fileData = nullptr;
    }
}

void Resource::release()
{
    Object::release();
    backendResource = nullptr;
}
