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

        // Set the text
        void setTextFormat(const TextFormat& format);
        void setText(const std::string& text);
        void setColor(const glm::vec3& color);

        // Render the text
        virtual void release() override;

        // Update the node, called each frame
        virtual void update(const NodeUpdateParameters& parameters) override;

    private:
        TextFormat format;
        std::string text;
        glm::vec3 color;
        std::vector<Handle<Mesh>> glyphs;
        std::unordered_map<char, Handle<TextMaterial>> dictionary;

        Handle<TextMaterial> getMaterialFor(char character);
    };
}
