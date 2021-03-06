#include "Mesh.h"
#include "BackendObjectFactory.h"
#include "Logs.h"
#include "materials/Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace coral;

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& textCoords, const glm::vec3& tangent, const glm::vec3& bitangent)
    : position(position)
    , normal(normal)
    , textCoords(textCoords)
    , tangent(tangent)
    , bitangent(bitangent)
{
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices)
    , indices(indices)
{
    connect<&Mesh::init>(Object::init, this);
    connect<&Mesh::release>(Object::release, this);
}

void Mesh::draw(const RenderParameters&)
{
    // Draw
    vertexBuffer->draw();
}

void Mesh::init()
{
    // Params
    backend::BackendVertexBufferParams params;

    // Data
    backend::BackendVertexBufferData data;
    data.verticesCount = static_cast<int>(vertices.size());
    data.vertices = vertices.data();
    data.indicesCount = static_cast<int>(indices.size());
    data.indices = indices.data();

    // Attributes
    data.vertexSize = sizeof(Vertex);
    std::vector<backend::BackendVertexAttribute> attributes {
        backend::BackendVertexAttribute { 3 }, // Positions
        backend::BackendVertexAttribute { 3 }, // Normals
        backend::BackendVertexAttribute { 2 }, // Texture coords
        backend::BackendVertexAttribute { 3 }, // Tangents
        backend::BackendVertexAttribute { 3 } // Bitangents
    };
    data.vertexAttributes = attributes;

    // Create buffer
    vertexBuffer = backend::BackendObjectFactory<backend::BackendVertexBuffer>::create(params, data);

    // Clear data holders
    vertices.clear();
    indices.clear();
}

void Mesh::release()
{
    vertexBuffer = nullptr;
}
