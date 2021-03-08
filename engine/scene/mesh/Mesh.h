#pragma once

#include "BackendVertexBuffer.h"
#include "scene/DrawableNode.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace coral
{
    struct MeshVertexBuffer
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textCoords;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;

        // Helpers
        void reserve(size_t size);
        size_t sizeOfVertex() const;
        void copyTo(std::vector<float>& data) const;
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
    };
}
