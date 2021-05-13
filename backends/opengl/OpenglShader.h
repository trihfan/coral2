#pragma once

#include "gl.h"
#include "BackendPipeline.h"
#include <array>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace backend::opengl
{
    // supported shader types
    enum ShaderType : int
    {
        vertex,
        fragment,
        geometry
    };

    class OpenglShader
    {
    public:
        // initialization
        OpenglShader(const std::string& name);
        ~OpenglShader();
        void init();

        // add a shader code
        void addShaderCode(ShaderType type, const std::string& code);
        void addUniformBlock(ShaderType type, const BackendUniformBlock& uniformBlock);

        // use the shader
        void use();
        GLuint getId() const;

        // utility uniform functions
        void setUniform(const std::string& name, bool value) const;
        void setUniform(const std::string& name, int value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, const glm::vec2& value) const;
        void setUniform(const std::string& name, const glm::vec3& value) const;
        void setUniform(const std::string& name, const glm::vec4& value) const;
        void setUniform(const std::string& name, const glm::mat2& mat) const;
        void setUniform(const std::string& name, const glm::mat3& mat) const;
        void setUniform(const std::string& name, const glm::mat4& mat) const;

        // utility uniform block functions
        void setBlockBufferSubData(const std::string& name, size_t offset, size_t size, void* data);

    private:
        std::string name;
        std::array<std::string, 3> shaderCode;
        std::array<std::vector<BackendUniformBlock>, 3> uniformBlocks;
        GLuint id = 0;
        std::array<std::vector<GLuint>, 3> uniformBlockIds;

        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        bool checkCompileErrors(GLuint shader, std::string type);
    };
}
