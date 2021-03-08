#include "Mesh.h"
#include "BackendObjectFactory.h"
#include "Logs.h"
#include "materials/Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace coral;

void MeshVertexBuffer::reserve(size_t size)
{
    positions.reserve(size);
    normals.reserve(size);
    textCoords.reserve(size);
    tangents.reserve(size);
    bitangents.reserve(size);
}

size_t MeshVertexBuffer::sizeOfVertex() const
{
    size_t size = 0;
    if (!positions.empty())
    {
        size += sizeof(float) * 3;
    }
    if (!normals.empty())
    {
        size += sizeof(float) * 3;
    }
    if (!textCoords.empty())
    {
        size += sizeof(float) * 2;
    }
    if (!tangents.empty())
    {
        size += sizeof(float) * 3;
    }
    if (!bitangents.empty())
    {
        size += sizeof(float) * 3;
    }
    return size;
}

void MeshVertexBuffer::copyTo(std::vector<float>& data) const
{
    size_t vertexSize = sizeOfVertex() / sizeof(float);
    data.resize(vertexSize * positions.size());

    for (size_t i = 0; i < positions.size(); i++)
    {
        size_t current = 0;
        if (!positions.empty())
        {
            std::memcpy(&data[i * vertexSize], &positions[i], 3 * sizeof(float));
            current += 3;
        }
        if (!normals.empty())
        {
            std::memcpy(&data[i * vertexSize + current], &normals[i], 3 * sizeof(float));
            current += 3;
        }
        if (!textCoords.empty())
        {
            std::memcpy(&data[i * vertexSize + current], &textCoords[i], 2 * sizeof(float));
            current += 2;
        }
        if (!tangents.empty())
        {
            std::memcpy(&data[i * vertexSize + current], &tangents[i], 3 * sizeof(float));
            current += 3;
        }
        if (!bitangents.empty())
        {
            std::memcpy(&data[i * vertexSize + current], &bitangents[i], 3 * sizeof(float));
            current += 3;
        }
    }
}

Mesh::Mesh(const MeshVertexBuffer& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices)
    , indices(indices)
{
    addRenderQueueTag(defaultRenderPassName);
}

void Mesh::draw(const RenderParameters&)
{
    // Draw
    vertexBuffer->draw();
}

void Mesh::init()
{
    DrawableNode::init();

    // Params
    backend::BackendVertexBufferParams params;

    // Data
    backend::BackendVertexBufferData data;

    // Copy vertices
    data.verticesCount = static_cast<int>(vertices.positions.size());
    std::vector<float> vertexData;
    vertices.copyTo(vertexData);
    data.vertices = vertexData.data();

    // Copy indices
    data.indicesCount = static_cast<int>(indices.size());
    data.indices = indices.data();

    // Attributes
    data.vertexSize = static_cast<int>(vertices.sizeOfVertex());
    std::vector<backend::BackendVertexAttribute> attributes;

    if (!vertices.positions.empty())
    {
        attributes.push_back(backend::BackendVertexAttribute { 3 });
    }
    if (!vertices.normals.empty())
    {
        attributes.push_back(backend::BackendVertexAttribute { 3 });
    }
    if (!vertices.textCoords.empty())
    {
        attributes.push_back(backend::BackendVertexAttribute { 2 });
    }
    if (!vertices.tangents.empty())
    {
        attributes.push_back(backend::BackendVertexAttribute { 3 });
    }
    if (!vertices.bitangents.empty())
    {
        attributes.push_back(backend::BackendVertexAttribute { 3 });
    }

    data.vertexAttributes = attributes;

    // Create buffer
    vertexBuffer = backend::BackendObjectFactory<backend::BackendVertexBuffer>::create(params, data);

    // Clear data holders
    vertices = MeshVertexBuffer();
    indices.clear();
}

void Mesh::release()
{
    DrawableNode::release();
    vertexBuffer = nullptr;
}
