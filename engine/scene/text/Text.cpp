#include "Text.h"
#include "base/ObjectFactory.h"
#include "resources/Resource.h"
#include "utils/Freetype.h"

using namespace coral;

Text::Text(const TextFormat& format)
    : format(format)
{
    addRenderQueueTag(defaultRenderPassName);
}

void Text::setTextFormat(const TextFormat& format)
{
    this->format = format;
    setText(text);
}

void Text::setText(const std::string& text)
{
    this->text = text;

    // Clear old text
    for (const auto& mesh : glyphs)
    {
        removeChild(mesh);
    }
    glyphs.clear();

    // Set new text
    int currentX = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        // Material
        auto material = getMaterialFor(text[i]);

        // Compute vertices
        MeshVertexBuffer vertices;
        if (format.mode == TextMode::text2d)
        {
            const float xpos = currentX + getTranslation().x + +material->getGlyph().bearing.x * getScale().x;
            const float ypos = getTranslation().y - (material->getGlyph().size.y - material->getGlyph().bearing.y) * getScale().y;
            const float w = material->getGlyph().size.x * getScale().x;
            const float h = material->getGlyph().size.y * getScale().y;
            currentX += (material->getGlyph().advance >> 6) * getScale().x;

            vertices.addPosition(glm::vec3(xpos, ypos + h, -0.1f));
            vertices.addPosition(glm::vec3(xpos, ypos, -0.1f));
            vertices.addPosition(glm::vec3(xpos + w, ypos, -0.1f));
            vertices.addPosition(glm::vec3(xpos, ypos + h, -0.1f));
            vertices.addPosition(glm::vec3(xpos + w, ypos, -0.1f));
            vertices.addPosition(glm::vec3(xpos + w, ypos + h, -0.1f));

            vertices.addTextCoord(glm::vec2(0, 0));
            vertices.addTextCoord(glm::vec2(0, 1));
            vertices.addTextCoord(glm::vec2(1, 1));
            vertices.addTextCoord(glm::vec2(0, 0));
            vertices.addTextCoord(glm::vec2(1, 1));
            vertices.addTextCoord(glm::vec2(1, 0));
        }
        else
        {
            Logs(error) << "Unimplemented text mode 3d";
        }

        const std::vector<unsigned int> indices { 0, 1, 2, 3, 4, 5 };

        // Create mesh
        Handle<Mesh> character = ObjectFactory::create<Mesh>(vertices, indices);
        character->setMaterial(material);

        // Set
        addChild(character);
    }
}

void Text::setColor(const glm::vec3& color)
{
    this->color = color;
    for (const auto& pair : dictionary)
    {
        pair.second->setColor(color);
    }
}

void Text::release()
{
    dictionary.clear();
}

Handle<TextMaterial> Text::getMaterialFor(char character)
{
    auto it = dictionary.find(character);
    if (it != dictionary.end())
    {
        return it->second;
    }

    TextMaterialParams params;
    params.character = character;
    params.font = format.font;
    params.mode = format.mode;
    params.size = format.size;
    auto material = ObjectFactory::create<TextMaterial>(getRenderQueueTags(), params);
    material->setColor(color);

    dictionary[character] = material;
    return material;
}
