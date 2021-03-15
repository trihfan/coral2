#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace coral
{

    /**
     * @brief The MeshVertexBuffer struct contains data for the mesh vertex buffer
     */
    struct MeshVertexBuffer
    {
        enum AttributeType : size_t
        {
            position,
            normal,
            textCoords,
            bone,
            weight,
            count
        };

        // Helpers
        MeshVertexBuffer();
        void reserve(size_t size);
        size_t sizeOfVertex() const;
        size_t vertexCount() const;
        size_t getComponentCount(AttributeType type) const;
        void copyTo(std::vector<float>& buffer) const;
        bool has(AttributeType type) const;
        int getLocation(AttributeType type) const;

        template <typename Type>
        void insert(AttributeType type, const Type& value)
        {
            size_t size = data[type].size();
            data[type].resize(size + sizeof(value) / sizeof(float));
            std::memcpy(&data[type][size], &value, sizeof(value));
        }

    private:
        std::array<std::vector<float>, count> data;
        std::array<size_t, count> dataSize;
    };
}
