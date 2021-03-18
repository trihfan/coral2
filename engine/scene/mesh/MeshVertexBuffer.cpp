#include "MeshVertexBuffer.h"
#include "Logs.h"

using namespace coral;

MeshVertexBuffer::MeshVertexBuffer()
{
    // Default values for data sizes
    for (size_t i = 0; i < count; i++)
    {
        switch (i)
        {
        case position:
        case normal:
            componentCount[i] = 3;
            break;

        case textCoords:
            componentCount[i] = 2;
            break;

        case bone:
        case weight:
            componentCount[i] = 4;
            break;

        default:
            componentCount[i] = 0;
            break;
        }
    }
}

void MeshVertexBuffer::reserve(size_t size)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i].reserve(size * componentCount[i] * sizeof(float));
    }
}

size_t MeshVertexBuffer::sizeOfVertex() const
{
    size_t size = 0;
    for (size_t i = 0; i < data.size(); i++)
    {
        if (!data[i].empty())
        {
            size += componentCount[i] * sizeof(float);
        }
    }
    return size;
}

size_t MeshVertexBuffer::vertexCount() const
{
    return data[position].size() / (componentCount[position] * sizeof(float));
}

void MeshVertexBuffer::copyTo(std::vector<std::byte>& buffer) const
{
    size_t vertexSize = sizeOfVertex();
    size_t vertexNumber = vertexCount();

    // Resize buffer
    buffer.resize(vertexNumber * vertexSize);

    // Copy data
    for (size_t currentVertex = 0; currentVertex < vertexNumber; currentVertex++)
    {
        size_t current = 0;
        for (size_t i = 0; i < data.size(); i++)
        {
            if (!data[i].empty())
            {
                size_t attributeSize = componentCount[i] * sizeof(float);
                std::memcpy(&buffer[currentVertex * vertexSize + current], &data[i][currentVertex * attributeSize], attributeSize);
                current += attributeSize;
            }
        }
    }
}

bool MeshVertexBuffer::hasAttribute(AttributeType type) const
{
    return !data[type].empty();
}

int MeshVertexBuffer::getLocation(AttributeType type) const
{
    int location = 0;
    for (size_t i = 0; i < type; i++)
    {
        if (!data[i].empty())
        {
            location++;
        }
    }
    return location;
}

size_t MeshVertexBuffer::getComponentCount(AttributeType type) const
{
    return componentCount[type];
}
