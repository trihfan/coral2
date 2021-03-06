#include "Resource.h"
#include "BackendObjectFactory.h"
#include "stb_image.h"

using namespace coral;

Resource::Resource(const ResourceParams& params)
    : params(params)
    , fileData(nullptr)
{
    connect<&Resource::init>(Object::init, this);
    connect<&Resource::release>(Object::release, this);
}

Resource::Resource(const std::string& file)
{
    connect<&Resource::init>(Object::init, this);
    connect<&Resource::release>(Object::release, this);

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
        params.format = backend::BackendResourceFormat::r8g8;
        break;

    case 3:
        params.format = backend::BackendResourceFormat::r8g8b8;
        break;

    case 4:
        params.format = backend::BackendResourceFormat::r8g8b8a8;
        break;

    default:
        Logs(error) << "Texture failed to load (wrong component size) at path: " << file;
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
    backendResource = nullptr;
}
