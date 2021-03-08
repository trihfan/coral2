#pragma once

#include "materials/GlyphMaterial.h"
#include "scene/Node.h"
#include "scene/mesh/Mesh.h"
#include <unordered_map>

namespace coral
{
    class Text : public Node
    {
    public:
        Text(const std::string& font);

        // Set the text
        void setText(const std::string& text, TextMode mode = TextMode::text2d);
        void setColor(const glm::vec3& color);

        // Render the text
        virtual void release() override;

    private:
        std::string font;
        std::string text;
        TextMode mode;
        glm::vec3 color;
        std::vector<Handle<Mesh>> glyphs;
        std::unordered_map<char, Handle<GlyphMaterial>> dictionary;

        Handle<GlyphMaterial> getMaterialFor(char character);
    };
}
