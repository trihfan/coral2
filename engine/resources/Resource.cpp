#include "Resource.h"
#include "Framebuffer.h"
#include "stb_image.h"

using namespace coral;

Resource::Resource() : ownImage(false)
{
    connect(params.modified, *this, &Resource::reset);
}

bool Resource::load(const std::string& file)
{
    int width, height;
    int nbComponents;
    fileData = stbi_load(file.c_str(), &width, &height, &nbComponents, 0);
    return updateParams(width, height, nbComponents);
}

bool Resource::load(const unsigned char* buffer, int length)
{
    int width, height;
    int nbComponents;
    fileData = stbi_load_from_memory(buffer, length, &width, &height, &nbComponents, 0);
    return updateParams(width, height, nbComponents);
}

bool Resource::updateParams(int width, int height, int nbComponents)
{
    if (!fileData)
    {
        Logs(error) << "Texture failed to load from memory";
        return false;
    }

    auto newParams = *params;
    switch (nbComponents)
    {
    case 1:
        newParams.format = ResourceFormat::r8;
        break;

    case 2:
        newParams.format = ResourceFormat::rg88;
        break;

    case 3:
        newParams.format = ResourceFormat::rgb888;
        break;

    case 4:
        newParams.format = ResourceFormat::rgba8888;
        break;

    default:
        Logs(error) << "Texture failed to load from memory";
        return false;
    }

    newParams.type = ResourceType::texture2d;
    newParams.width = width;
    newParams.height = height;
    params = newParams;

    reset();
    return true;
}

void Resource::init()
{
    // When referencing the backbuffer, the resource become a mock for referencing the backbuffer resource
    if (name == backbufferName)
    {
        return;
    }

    // Todo

    if (fileData)
    {
        stbi_image_free(fileData);
        fileData = nullptr;
    }
}

void Resource::release()
{
    vkDestroyImageView(device.logicalDevice, image.imageView, nullptr);
    if (ownImage)
    {
        vkDestroyImage(device.logicalDevice, image.image, nullptr);
    }
}
