#pragma once

#include "BackendVertexBuffer.h"
#include "materials/Material.h"
#include "scene/DrawableNode.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace coral
{
    enum class MeshShaderAttributeType : int
    {
        position,
        normal,
        textCoords,
        bone,
        weight,
        count
    };

    /**
     * @brief The MeshVertexBuffer struct contains data for the mesh vertex buffer
     */
    struct MeshVertexBuffer
    {
        std::array<std::vector<float>, size_t(MeshShaderAttributeType::count)> data;
        std::array<size_t, size_t(MeshShaderAttributeType::count)> dataSize;

        // Helpers
        MeshVertexBuffer();
        void reserve(size_t size);
        size_t sizeOfVertex() const;
        void copyTo(std::vector<float>& buffer) const;
        void insert(MeshShaderAttributeType type, glm::vec1 value);
        void insert(MeshShaderAttributeType type, glm::vec2 value);
        void insert(MeshShaderAttributeType type, glm::vec3 value);
        void insert(MeshShaderAttributeType type, glm::vec4 value);

        bool has(MeshShaderAttributeType type) const;
        int getLocation(MeshShaderAttributeType type) const;
    };

    /**
     * @brief A mesh drawable node using vertices and indices
     */
    class Mesh : public DrawableNode
    {
    public:
        // constructor
        Mesh(const MeshVertexBuffer& vertices, const std::vector<unsigned int>& indices);

        // render the mesh
        virtual void draw(const RenderParameters& parameters) override;
        virtual void init() override;
        virtual void release() override;

    private:
        // Data holders
        MeshVertexBuffer vertices;
        std::vector<unsigned int> indices;

        // Buffers
        std::unique_ptr<backend::BackendVertexBuffer> vertexBuffer;

        // initializes all the buffer objects/arrays
        void setupMesh();

        std::vector<backend::BackendVertexAttribute> createAttributeArray() const;
    };
}
