#pragma once

#include "Material.h"
#include "utils/Freetype.h"

namespace coral
{
    enum class TextMode
    {
        text2d,
        text3d
    };

    struct GlyphMaterialParams
    {
        char character;
        std::string font;
        TextMode mode;
        int size;
    };

    class GlyphMaterial : public Material
    {
    public:
        GlyphMaterial(const std::vector<std::string>& renderQueueTags, const GlyphMaterialParams& params);

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        virtual void init() override;
        virtual void release() override;

        void setColor(const glm::vec3& color);
        const FreetypeGlyph& getGlyph() const;

    protected:
        virtual Handle<Pipeline> createPipelineFor(const std::string& renderpass) override;
        virtual std::string getPipelineName() const override;

    private:
        glm::vec3 color;
        GlyphMaterialParams params;
        FreetypeGlyph glyph;
        Handle<Resource> resource;
        glm::mat4 projection;
        bool dirty;
    };
}
