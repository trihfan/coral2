#include "Text.h"
#include "ObjectFactory.h"
#include "materials/BasicMaterial.h"
#include "resources/Resource.h"
#include "utils/Freetype.h"

using namespace coral;

Text::Text(const std::string& font)
    : font(font)
{
    addRenderQueueTag(defaultRenderPassName);
}

void Text::setText(const std::string& text, TextMode mode)
{
    this->text = text;
    this->mode = mode;

    // Clear old text
    for (const auto& mesh : glyphs)
    {
        removeChild(mesh);
    }
    glyphs.clear();

    // Set new text
    for (size_t i = 0; i < text.size(); i++)
    {
        // Material
        auto material = getMaterialFor(text[i]);

        // Compute vertices
        MeshVertexBuffer vertices;
        if (mode == TextMode::text2d)
        {
            const float xpos = getTranslation().x + i * (material->getGlyph().advance >> 6) * getScale().x + material->getGlyph().bearing.x * getScale().x;
            const float ypos = getTranslation().y - (material->getGlyph().size.y - material->getGlyph().bearing.y) * getScale().y;
            const float w = material->getGlyph().size.x * getScale().x;
            const float h = material->getGlyph().size.y * getScale().y;

            vertices.positions = { glm::vec3(xpos, ypos + h, 0), glm::vec3(xpos, ypos, 0), glm::vec3(xpos + w, ypos, 0), glm::vec3(xpos, ypos + h, 0), glm::vec3(xpos + w, ypos, 0), glm::vec3(xpos + w, ypos + h, 0) };
            vertices.textCoords = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f) };
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

Handle<GlyphMaterial> Text::getMaterialFor(char character)
{
    auto it = dictionary.find(character);
    if (it != dictionary.end())
    {
        return it->second;
    }

    GlyphMaterialParams params;
    params.character = character;
    params.font = font;
    params.mode = mode;
    auto material = ObjectFactory::create<GlyphMaterial>(getRenderQueueTags(), params);
    material->setColor(color);

    dictionary[character] = material;
    return material;
}
