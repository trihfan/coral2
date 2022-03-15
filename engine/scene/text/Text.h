#pragma once

#include "materials/TextMaterial.h"
#include "scene/Node.h"
#include "scene/mesh/Mesh.h"
#include <unordered_map>

namespace coral
{
    struct TextFormat
    {
        std::string font;
        TextMode mode = TextMode::text2d;
        int size = 16;
    };

    class Text : public Node
    {
    public:
        Text(const TextFormat& format);

        // Properties
        Property<TextFormat> format;
        Property<std::string> text;
        Property<glm::vec3> color;

        // Render the text
        virtual void release() override;

        // Update the node, called each frame
        virtual void update() override;

    private:
        std::vector<Handle<Mesh>> glyphs;
        std::unordered_map<char, Handle<TextMaterial>> dictionary;

        // Create character material
        Handle<TextMaterial> getMaterialFor(char character);
    };
}
