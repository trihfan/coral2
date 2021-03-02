#ifndef MESH_H
#define MESH_H

#include "BackendVertexBuffer.h"
#include "scene/DrawableNode.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace coral
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textCoords;
        glm::vec3 tangent;
        glm::vec3 bitTangent;

        Vertex(const glm::vec3& position = glm::vec3(), const glm::vec3& normal = glm::vec3(), const glm::vec3& textCoords = glm::vec3(), const glm::vec3& tangent = glm::vec3(), const glm::vec3& bitTangent = glm::vec3())
            : position(position)
            , normal(normal)
            , textCoords(textCoords)
            , tangent(tangent)
            , bitTangent(bitTangent)
        {
        }
    };

    class Mesh : public DrawableNode
    {
    public:
        // constructor
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        // render the mesh;
        virtual void draw(const RenderParameters& parameters) override;

    private:
        // Data holders
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Buffers
        std::unique_ptr<backend::BackendVertexBuffer> vertexBuffer;

        // initializes all the buffer objects/arrays
        void setupMesh();

        // slots
        void init();
        void release();
    };
}
#endif
