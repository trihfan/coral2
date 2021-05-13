#pragma once

#include <array>
#include <vector>
#include "BackendPipeline.h"

namespace coral
{
    struct ShaderAttribute
    {
        std::string name;
        int location;
        std::string type;
    };

    class ShaderComposer
    {
    public:
        ShaderComposer(const std::string& shaderfile, int glslVersion);
        void addAttribute(const ShaderAttribute& attribute);
        void addDefinition(const std::string& definition);

        // Build the shaders
        void process();

        // Return the shader contents
        std::string getVertexShader() const;
        std::string getFragmentShader() const;

        // Return the uniform block info
        const std::vector<backend::BackendUniformBlock>& getVertexShaderUniformBlocks() const;
        const std::vector<backend::BackendUniformBlock>& getFragmentShaderUniformBlocks() const;

    private:
        // Parameter
        std::string shaderFile;
        int glslVersion;
        std::vector<ShaderAttribute> attributes;
        std::vector<std::string> definitions;
        std::array<std::vector<backend::BackendUniformBlock>, 2> uniformBlocks;

        // Parser
        bool basicLighting;
        int maxLightCount;

        // Output
        std::string vertexContent;
        std::string fragmentContent;

        void parseParameters(const std::string& content);
        void parseVertexShader(const std::string& content);
        void parseFragmentShader(const std::string& content);
        void parseUniformBlock(std::string& content, size_t index);
        void parseInputOutput(std::string& content);
    };
}
