#include "MeshVertexBuffer.h"
#include "Logs.h"
#include "materials/MeshMaterial.h"

using namespace coral;

static const std::vector<size_t> componentCount {
    3, 3, 2, 4, 4, 4, 4, 4, 4
};

MeshVertexBuffer::MeshVertexBuffer()
    : boneIncidenceWarningSend(false)
{
    assert(componentCount.size() == count);
}

void MeshVertexBuffer::reserve(size_t size)
{
    positions.reserve(size);
    normals.reserve(size);
    texCoords.reserve(size);
    boneIncidences0.reserve(size);
    boneIncidences1.reserve(size);
    boneIncidences2.reserve(size);
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

std::vector<std::byte> MeshVertexBuffer::pack() const
{
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

    case boneId0:
    case boneWeight0:
        return !boneIncidences0.empty();

    case boneId1:
    case boneWeight1:
        return !boneIncidences1.empty();

    case boneId2:
    case boneWeight2:
        return !boneIncidences2.empty();

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

    // Fill the first available slot or return false
    auto fillBoneIncidence = +[](std::vector<std::pair<glm::vec4, glm::vec4>>& boneIncidences, size_t verticeIndex, int id, float weight, size_t vertexCount) {
        // Resize so vertice index fit inside
        if (boneIncidences.size() < vertexCount)
        {
            boneIncidences.resize(vertexCount, std::make_pair(glm::vec4(MeshMaterial::maxBones, MeshMaterial::maxBones, MeshMaterial::maxBones, MeshMaterial::maxBones), glm::vec4(0)));
        }

        // Check if there is an empty slot
        for (int i = 0; i < 4; i++)
        {
            if (static_cast<int>(boneIncidences[verticeIndex].first[i]) == MeshMaterial::maxBones)
            {
                std::memcpy(&boneIncidences[verticeIndex].first[i], &id, sizeof(int));
                boneIncidences[verticeIndex].second[i] = weight;
                return true;
            }
        }
        return false;
    };

    // Try all bone incidences
    if (fillBoneIncidence(boneIncidences0, verticeIndex, id, weight, vertexCount())
        || fillBoneIncidence(boneIncidences1, verticeIndex, id, weight, vertexCount())
        || fillBoneIncidence(boneIncidences2, verticeIndex, id, weight, vertexCount()))
    {
        return;
    }

    // If no more space available for bone indicence, show a warning
    if (!boneIncidenceWarningSend)
    {
        boneIncidenceWarningSend = true;
        Logs(warning) << "Too much bones incidence for one vertex";
    }
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

    case boneId0:
        return &boneIncidences0[index].first;

    case boneWeight0:
        return &boneIncidences0[index].second;

    case boneId1:
        return &boneIncidences1[index].first;

    case boneWeight1:
        return &boneIncidences1[index].second;

    case boneId2:
        return &boneIncidences2[index].first;

    case boneWeight2:
        return &boneIncidences2[index].second;

    default:
        return nullptr;
    }
}
