#include "MeshVertexBuffer.h"
#include "Logs.h"
#include "materials/MeshMaterial.h"

using namespace coral;

static const std::vector<size_t> componentCount {
    3, 3, 2, 4, 4
};

MeshVertexBuffer::MeshVertexBuffer()
{
    assert(componentCount.size() == count);
}

void MeshVertexBuffer::reserve(size_t size)
{
    positions.reserve(size);
    normals.reserve(size);
    texCoords.reserve(size);
}

size_t MeshVertexBuffer::sizeOfVertex() const
{
    size_t size = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (hasAttribute(static_cast<AttributeType>(i)))
        {
            size += getComponentCount(static_cast<AttributeType>(i));
        }
    }
    return size * sizeof(float);
}

size_t MeshVertexBuffer::vertexCount() const
{
    return positions.size();
}

std::vector<std::byte> MeshVertexBuffer::pack()
{
    // Setup
    packBoneIncidences();

    size_t vertexSize = sizeOfVertex();
    size_t vertexNumber = vertexCount();

    // Resize buffer
    std::vector<std::byte> buffer(vertexNumber * vertexSize);

    // Copy data
    for (size_t currentVertex = 0; currentVertex < vertexNumber; currentVertex++)
    {
        size_t current = 0;
        for (size_t i = 0; i < componentCount.size(); i++)
        {
            if (hasAttribute(static_cast<AttributeType>(i)))
            {
                size_t attributeSize = getComponentCount(static_cast<AttributeType>(i)) * sizeof(float);
                std::memcpy(&buffer[currentVertex * vertexSize + current], getPtrTo(static_cast<AttributeType>(i), currentVertex), attributeSize);
                current += attributeSize;
            }
        }
    }
    return buffer;
}

bool MeshVertexBuffer::hasAttribute(AttributeType type) const
{
    switch (type)
    {
    case position:
        return !positions.empty();

    case normal:
        return !normals.empty();

    case textCoords:
        return !texCoords.empty();

    case boneId:
    case boneWeight:
        return !boneIncidences.empty();

    default:
        return false;
    }
}

int MeshVertexBuffer::getLocation(AttributeType type) const
{
    int location = 0;
    for (size_t i = 0; i < type; i++)
    {
        if (hasAttribute(static_cast<AttributeType>(i)))
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

void MeshVertexBuffer::addPosition(const glm::vec3& position)
{
    positions.push_back(position);
}

void MeshVertexBuffer::addNormal(const glm::vec3& normal)
{
    normals.push_back(normal);
}

void MeshVertexBuffer::addTextCoord(const glm::vec2& textCoords)
{
    texCoords.push_back(textCoords);
}

void MeshVertexBuffer::addBoneIncidence(size_t verticeIndex, int id, float weight)
{
    if (id >= MeshMaterial::maxBones)
    {
        Logs(warning) << "bone id (" << id << ") exceed the limit of " << MeshMaterial::maxBones;
        return;
    }

    // Resize if necessary
    if (boneIncidences.size() < std::max(vertexCount(), verticeIndex))
    {
        boneIncidences.resize(std::max(vertexCount(), verticeIndex));
    }

    // Put the incidence
    boneIncidences[verticeIndex].push_back(std::make_pair(id, weight));
}

const void* MeshVertexBuffer::getPtrTo(AttributeType type, size_t index) const
{
    switch (type)
    {
    case position:
        return &positions[index];

    case normal:
        return &normals[index];

    case textCoords:
        return &texCoords[index];

    case boneId:
        return &packedBoneIncidences[index].first;

    case boneWeight:
        return &packedBoneIncidences[index].second;

    default:
        return nullptr;
    }
}

void MeshVertexBuffer::packBoneIncidences()
{
    if (boneIncidences.empty())
    {
        return;
    }

    bool warningSend = false;
    packedBoneIncidences.resize(vertexCount());
    for (size_t i = 0; i < packedBoneIncidences.size(); i++)
    {
        // Sort by weight so we keep only the heaviest weight in case there is more than 4 incidences
        std::sort(boneIncidences[i].begin(), boneIncidences[i].end(), [](const auto& a, const auto& b) { return a.second >= b.second; });
        for (int j = 0; j < 4; j++)
        {
            // Bone incidence
            if (static_cast<size_t>(j) < boneIncidences[i].size())
            {
                std::memcpy(&packedBoneIncidences[i].first[j], &boneIncidences[i][static_cast<size_t>(j)].first, sizeof(int));
                packedBoneIncidences[i].second[j] = boneIncidences[i][static_cast<size_t>(j)].second;
            }
            // Default value
            else
            {
                std::memcpy(&packedBoneIncidences[i].first[j], &MeshMaterial::maxBones, sizeof(int));
            }
        }

        // Warning in case there is too much incidences for the vertex
        if (boneIncidences[i].size() > 4 && !warningSend)
        {
            Logs(warning) << "Bone incidence count on the vertex exceed 4";
            warningSend = true;
        }
    }
}
