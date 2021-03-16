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
            attributeSize[i] = componentCount[i] * sizeof(float);
            break;

        case textCoords:
            componentCount[i] = 2;
            attributeSize[i] = componentCount[i] * sizeof(float);
            break;

        case bone:
            componentCount[i] = 4;
            attributeSize[i] = componentCount[i] * sizeof(int);
            break;

        case weight:
            componentCount[i] = 4;
            attributeSize[i] = componentCount[i] * sizeof(float);
            break;

        default:
            componentCount[i] = 0;
            attributeSize[i] = componentCount[i] * sizeof(float);
            break;
        }
    }
}

void MeshVertexBuffer::reserve(size_t size)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i].reserve(size * attributeSize[i]);
    }
}

size_t MeshVertexBuffer::sizeOfVertex() const
{
    size_t size = 0;
    for (size_t i = 0; i < data.size(); i++)
    {
        if (!data[i].empty())
        {
            size += attributeSize[i];
        }
    }
    return size;
}

size_t MeshVertexBuffer::vertexCount() const
{
    return data[position].size() / (attributeSize[position]);
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
                std::memcpy(&buffer[currentVertex * vertexSize + current], &data[i][currentVertex * attributeSize[i]], attributeSize[i]);
                current += attributeSize[i];
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
