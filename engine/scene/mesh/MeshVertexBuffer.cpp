#include "MeshVertexBuffer.h"

using namespace coral;

MeshVertexBuffer::MeshVertexBuffer()
{
    // Default values for data sizes
    for (size_t i = 0; i < dataSize.size(); i++)
    {
        switch (i)
        {
        case position:
            dataSize[i] = 3;
            break;

        case normal:
            dataSize[i] = 3;
            break;

        case textCoords:
            dataSize[i] = 2;
            break;

        case bone:
            dataSize[i] = 4;
            break;

        case weight:
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

size_t MeshVertexBuffer::vertexCount() const
{
    return data[position].size() / dataSize[position];
}

void MeshVertexBuffer::copyTo(std::vector<float>& buffer) const
{
    size_t componentCount = this->sizeOfVertex() / sizeof(float);
    size_t positionCount = vertexCount();

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

bool MeshVertexBuffer::has(AttributeType type) const
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
    return dataSize[type];
}
