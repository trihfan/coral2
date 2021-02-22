#pragma once

#include "OpenglBackend.h"

#include <array>
#include <glm/glm.hpp>
#include <string>

namespace coral
{
    class Shader
    {
    public:
        // supported shader types
        enum ShaderType : int
        {
            vertex,
            fragment,
            geometry
        };

        // add a shader data
        void addShaderData(ShaderType type, const std::string& data);

        // initialization
        void init();
        ~Shader();

        // use the shader
        // ------------------------------------------------------------------------
        void use();
        GLuint getId() const;

        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        std::array<std::string, 3> shader_data;
        GLuint id = 0;

        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(GLuint shader, std::string type);
    };
}
