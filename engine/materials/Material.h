#pragma once

#include "base/Object.h"
#include "base/Ptr.h"
#include "resources/Pipeline.h"
#include <memory>
#include <string>
#include <vector>

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
        ptr<Pipeline> getPipeline() const;
        void invalidatePipeline();

        // setup the shader and its parameters
        virtual void init() override;
        virtual void use(const RenderParameters& parameters);
        virtual void setNode(ptr<Node> node);

    protected:
        // Create the pipeline for the material compatible with the given renderpass
        virtual ptr<Pipeline> createPipelineFor(const std::string& renderpass) = 0;
        virtual std::string getPipelineName() const = 0;

        void enableLighting();

    private:
        std::vector<std::string> renderQueueTags;

        // Material params
        bool lighting;

        // The material pipeline
        ptr<Pipeline> pipeline;

        // Bind the lights
        void setupLights(const RenderParameters& parameters);
    };
}
