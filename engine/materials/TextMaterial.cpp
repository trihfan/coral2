#include "TextMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "RenderParameters.h"
#include "base/ObjectFactory.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "resources/ShaderComposer.h"
#include "scene/camera/Camera.h"
#include "utils/Freetype.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

TextMaterial::TextMaterial(const std::vector<std::string>& renderQueueTags, const TextMaterialParams& params)
    : Material(renderQueueTags)
    , params(params)
    , dirty(true)
{
    FreetypeGlyphParams freetypeParams;
    freetypeParams.font = params.font;
    freetypeParams.character = params.character;
    freetypeParams.size = params.size;
    glyph = Freetype::getGlyph(freetypeParams);
}

void TextMaterial::use(const RenderParameters& params)
{
    if (this->params.mode == TextMode::text2d && dirty)
    {
        projection = glm::ortho(0.f, static_cast<float>(params.width), 0.f, static_cast<float>(params.height), 0.1f, 100.f);
        dirty = false;
    }

    auto pipeline = getPipeline();
    pipeline->setUniform("projectionMatrix", projection);
    pipeline->setUniform("color", color);
    pipeline->setUniform("glyph", 0);
    resource->bind(0);
}

void TextMaterial::init()
{
    Material::init();

    // Generate texture
    ResourceParams resourceParams;
    resourceParams.type = backend::BackendResourceType::texture2d;
    resourceParams.format = backend::BackendResourceFormat::r8;
    resourceParams.width = glyph.size.x;
    resourceParams.height = glyph.size.y;
    resourceParams.data = std::move(glyph.data);
    resource = ObjectFactory::create<Resource>(resourceParams);
}

void TextMaterial::release()
{
    Material::release();
    glyph = FreetypeGlyph();
    resource = nullptr;
}

void TextMaterial::setColor(const glm::vec3& color)
{
    this->color = color;
}

ptr<Pipeline> TextMaterial::createPipelineFor(const std::string& renderpass)
{
    dirty = true;
    PipelineParams params;
    params.params.name = getPipelineName();
    params.renderpass = renderpass;

    ShaderComposer composer("assets/shaders/text_material.shader");

    // Attributes
    composer.addAttribute(ShaderAttribute { "position", 0, "vec3" });
    composer.addAttribute(ShaderAttribute { "textCoords", 1, "vec2" });

    // Process
    composer.process();
    params.params.vertexShader = composer.getVertexShader();
    params.params.fragmentShader = composer.getFragmentShader();

    params.params.depthTest = this->params.mode == TextMode::text3d;
    params.params.blending = true;
    return PipelineManager::createPipeline(params);
}

std::string TextMaterial::getPipelineName() const
{
    return "TextMaterialPipeline";
}

const FreetypeGlyph& TextMaterial::getGlyph() const
{
    return glyph;
}
