#ifndef MATERIAL_H
#define MATERIAL_H

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include "Object.h"
#include "utils/Shader.h"

namespace coral
{
    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
    public:
        // construction
        Material() = default;
        virtual ~Material() = default;

        // initialization
        virtual void init() override;
        virtual void release() override;

        // setup the shader and its parameters
        virtual void use();

    protected:
        // tmp
        unsigned int textureFromFile(const std::string& path, bool gamma = false);

        // add a shader data
        void addShaderData(ShaderType type, const std::string& data);

        // Get the shader
        std::shared_ptr<Shader> getShader();

    private:
        // the material shader
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    };

    /*class MaterialInstance
    {
        friend class Engine;
    public:
        static std::shared_ptr<Material> createMaterial(const std::string& alias = "");

    private:
        static std::vector<std::shared_ptr<Material>> materials;
        static std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    };*/
}
#endif