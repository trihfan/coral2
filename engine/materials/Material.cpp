#include <glad/glad.h>
#include "Material.h"

using namespace coral;

void Material::init()
{
    shader->init();
}

void Material::release()
{
    shader->release();
}

void Material::use()
{
    shader->use();
}

void Material::addShaderData(ShaderType type, const std::string& data)
{
    shader->addShaderData(type, data);
}

std::shared_ptr<Shader> Material::getShader()
{
    return shader;
}

#include "utils/Logs.h"
#include <stb_image.h>
unsigned int Material::textureFromFile(const std::string& path, bool gamma)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        Logs(error) << "Texture failed to load at path: " << path;
        stbi_image_free(data);
    }

    return textureID;
}