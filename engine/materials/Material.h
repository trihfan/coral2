#pragma once

#include "Object.h"
#include "resources/Pipeline.h"
#include <memory>
#include <string>

namespace coral
{
    struct RenderParameters;
    class Node;

    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
    public:
        Material(const std::vector<std::string>& renderQueueTags);

        // Pipeline
        Handle<Pipeline> getPipeline() const;

        // setup the shader and its parameters
        virtual void init() override;
        virtual void update() override;
        virtual void use(const RenderParameters& parameters);
        virtual void setNode(Handle<Node> node);

    protected:
        // Create the pipeline for the material compatible with the given renderpass
        virtual Handle<Pipeline> createPipelineFor(const std::string& renderpass) = 0;
        virtual std::string getPipelineName() const = 0;

        void enableLighting();

    private:
        std::vector<std::string> renderQueueTags;

        // Material params
        bool lighting;

        // The material pipeline
        Handle<Pipeline> pipeline;

        // Bind the lights
        void setupLights(const RenderParameters& parameters);
    };
}
