#include "GlyphMaterial.h"
#include "AssetManager.h"
#include "ObjectFactory.h"
#include "RenderParameters.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/camera/Camera.h"
#include "utils/Freetype.h"

using namespace coral;

GlyphMaterial::GlyphMaterial(const std::vector<std::string>& renderQueueTags, const GlyphMaterialParams& params)
    : Material(renderQueueTags)
    , params(params)
{
    FreetypeGlyphParams freetypeParams;
    freetypeParams.font = params.font;
    freetypeParams.character = params.character;
    glyph = Freetype::getGlyph(freetypeParams);
}

void GlyphMaterial::use(const RenderParameters& params)
{
    getPipeline()->setUniform("projection", this->params.mode == TextMode::text3d ? params.camera->getProjectionMatrix() : glm::mat4(1));
    getPipeline()->setUniform("color", color);
    getPipeline()->setUniform("glyph", 0);
    resource->bind(0);
}

void GlyphMaterial::setNode(Handle<Node>)
{
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

Handle<Pipeline> GlyphMaterial::getPipelineFor(const std::string& renderpass)
{
    PipelineParams params;
    params.name = "glyph_material";
    params.renderpass = renderpass;
    params.vertexShaderFile = AssetManager::getShader("glyph_material", ShaderType::vertex).asset.url;
    params.fragmentShaderFile = AssetManager::getShader("glyph_material", ShaderType::fragment).asset.url;
    params.params.depthTest = this->params.mode == TextMode::text3d;
    params.params.blending = true;
    return PipelineManager::getPipeline(params);
}

const FreetypeGlyph& GlyphMaterial::getGlyph() const
{
    return glyph;
}
