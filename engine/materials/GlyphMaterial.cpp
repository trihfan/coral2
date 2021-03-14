#include "GlyphMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "ObjectFactory.h"
#include "RenderParameters.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/camera/Camera.h"
#include "utils/Freetype.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

GlyphMaterial::GlyphMaterial(const std::vector<std::string>& renderQueueTags, const GlyphMaterialParams& params)
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

void GlyphMaterial::use(const RenderParameters& params)
{
    if (this->params.mode == TextMode::text2d && dirty)
    {
        projection = glm::ortho(0.f, static_cast<float>(params.width), 0.f, static_cast<float>(params.height), 0.1f, 100.f);
        dirty = false;
    }

    getPipeline()->setUniform("projection", this->params.mode == TextMode::text2d ? projection : params.camera->getProjectionMatrix() * params.camera->getViewMatrix());
    getPipeline()->setUniform("color", color);
    getPipeline()->setUniform("glyph", 0);
    resource->bind(0);
}

void GlyphMaterial::init()
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

void GlyphMaterial::release()
{
    Material::release();
    glyph = FreetypeGlyph();
    resource = nullptr;
}

void GlyphMaterial::setColor(const glm::vec3& color)
{
    this->color = color;
}

Handle<Pipeline> GlyphMaterial::createPipelineFor(const std::string& renderpass)
{
    dirty = true;
    PipelineParams params;
    params.params.name = getPipelineName();
    params.renderpass = renderpass;
    params.params.vertexShaderFile = AssetManager::getShader("glyph_material", ShaderType::vertex).asset.url;
    params.params.fragmentShaderFile = AssetManager::getShader("glyph_material", ShaderType::fragment).asset.url;
    params.params.depthTest = this->params.mode == TextMode::text3d;
    params.params.blending = true;
    return PipelineManager::createPipeline(params);
}

std::string GlyphMaterial::getPipelineName() const
{
    return "GlyphMaterialPipeline";
}

const FreetypeGlyph& GlyphMaterial::getGlyph() const
{
    return glyph;
}
