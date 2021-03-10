#pragma once

#include "Object.h"
#include "resources/Pipeline.h"
#include <memory>
#include <string>

namespace coral
{
    struct RenderParameters;
    class Node;

    enum class ShaderAttributeType : int
    {
        position,
        normal,
        textCoords,
        tangent,
        bitangent,
        bone,
        weight,
        count
    };

    struct ShaderAttribute
    {
        ShaderAttributeType type;
        int location;
    };

    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
    public:
        Material(const std::vector<std::string>& renderQueueTags);

        // Pipeline
        Handle<Pipeline> getPipeline() const;

        // Return the attributes of the material
        virtual std::vector<ShaderAttribute> getAttributes() const = 0;

        // setup the shader and its parameters
        virtual void init() override;
        virtual void update() override;
        virtual void use(const RenderParameters& parameters) = 0;
        virtual void setNode(Handle<Node> node) = 0;

    protected:
        // Bind the lights
        void setupLights(const RenderParameters& parameters);

        // Create the pipeline for the material compatible with the given renderpass
        virtual Handle<Pipeline> getPipelineFor(const std::string& renderpass) = 0;

    private:
        std::vector<std::string> renderQueueTags;

        // The material pipeline
        Handle<Pipeline> pipeline;
    };
}
