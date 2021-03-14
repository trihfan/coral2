#include "Mesh.h"
#include "BackendObjectFactory.h"
#include "Logs.h"
#include "materials/Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace coral;

MeshVertexBuffer::MeshVertexBuffer()
{
    // Default values for data sizes
    for (size_t i = 0; i < dataSize.size(); i++)
    {
        switch (MeshShaderAttributeType(i))
        {
        case MeshShaderAttributeType::position:
            dataSize[i] = 3;
            break;

        case MeshShaderAttributeType::normal:
            dataSize[i] = 3;
            break;

        case MeshShaderAttributeType::textCoords:
            dataSize[i] = 2;
            break;

        case MeshShaderAttributeType::bone:
            dataSize[i] = 4;
            break;

        case MeshShaderAttributeType::weight:
            dataSize[i] = 4;
            break;

        default:
            dataSize[i] = 0;
            break;
        }
    }
}

void MeshVertexBuffer::reserve(size_t size)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i].reserve(size * dataSize[i]);
    }
}

size_t MeshVertexBuffer::sizeOfVertex() const
{
    size_t size = 0;
    for (size_t i = 0; i < data.size(); i++)
    {
        size += data[i].empty() ? 0 : dataSize[i];
    }
    return size * sizeof(float);
}

void MeshVertexBuffer::copyTo(std::vector<float>& buffer) const
{
    size_t componentCount = this->sizeOfVertex() / sizeof(float);
    size_t positionCount = data[size_t(MeshShaderAttributeType::position)].size() / dataSize[size_t(MeshShaderAttributeType::position)];

    // Resize buffer
    buffer.resize(componentCount * positionCount);

    // Copy data
    for (size_t currentVertex = 0; currentVertex < positionCount; currentVertex++)
    {
        size_t current = 0;
        for (size_t i = 0; i < data.size(); i++)
        {
            if (!data[i].empty())
            {
                std::memcpy(&buffer[currentVertex * componentCount + current], &data[i][currentVertex * dataSize[i]], dataSize[i] * sizeof(float));
                current += dataSize[i];
            }
        }
    }
}

void MeshVertexBuffer::insert(MeshShaderAttributeType type, glm::vec1 value)
{
    data[size_t(type)].push_back(value.x);
}

void MeshVertexBuffer::insert(MeshShaderAttributeType type, glm::vec2 value)
{
    data[size_t(type)].push_back(value.x);
    data[size_t(type)].push_back(value.y);
}

void MeshVertexBuffer::insert(MeshShaderAttributeType type, glm::vec3 value)
{
    data[size_t(type)].push_back(value.x);
    data[size_t(type)].push_back(value.y);
    data[size_t(type)].push_back(value.z);
}

void MeshVertexBuffer::insert(MeshShaderAttributeType type, glm::vec4 value)
{
    data[size_t(type)].push_back(value.x);
    data[size_t(type)].push_back(value.y);
    data[size_t(type)].push_back(value.z);
    data[size_t(type)].push_back(value.w);
}

bool MeshVertexBuffer::has(MeshShaderAttributeType type) const
{
    return !data[size_t(type)].empty();
}

int MeshVertexBuffer::getLocation(MeshShaderAttributeType type) const
{
    int location = 0;
    for (size_t i = 0; i < size_t(type); i++)
    {
        if (!data[i].empty())
        {
            location++;
        }
    }
    return location;
}

/**********************************************************/

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

    // Data
    backend::BackendVertexBufferData data;

    // Copy vertices
    data.verticesCount = static_cast<int>(vertices.data[size_t(MeshShaderAttributeType::position)].size() / vertices.dataSize[size_t(MeshShaderAttributeType::position)]);
    std::vector<float> vertexData;
    vertices.copyTo(vertexData);
    data.vertices = vertexData.data();

    // Copy indices
    data.indicesCount = static_cast<int>(indices.size());
    data.indices = indices.data();

    // Attributes
    data.vertexSize = static_cast<int>(vertices.sizeOfVertex());
    data.vertexAttributes = createAttributeArray();

    // Create buffer
    vertexBuffer = backend::BackendObjectFactory<backend::BackendVertexBuffer>::create(data);
}

std::vector<backend::BackendVertexAttribute> Mesh::createAttributeArray() const
{
    std::vector<backend::BackendVertexAttribute> attributes;
    if (!material)
    {
        return attributes;
    }

    for (size_t i = 0; i < vertices.data.size(); i++)
    {
        if (vertices.has(MeshShaderAttributeType(i)))
        {
            backend::BackendVertexAttribute attribute;
            attribute.size = static_cast<int>(vertices.dataSize[i]);
            attribute.location = static_cast<int>(i);
            attributes.push_back(attribute);
        }
    }
    return attributes;
}

void Mesh::release()
{
    DrawableNode::release();
    vertexBuffer = nullptr;
}
