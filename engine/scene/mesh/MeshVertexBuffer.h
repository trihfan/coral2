#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace coral
{

    /**
     * @brief The MeshVertexBuffer struct contains data for the mesh vertex buffer
     */
    class MeshVertexBuffer
    {
    public:
        enum AttributeType : size_t
        {
            position,
            normal,
            textCoords,
            bone,
            weight,
            count
        };

        // Constructor
        MeshVertexBuffer();

        /**
         * @brief Reserve data in each attribute array
         */
        void reserve(size_t size);

        /**
         * @brief Return the size of one vertex
         */
        size_t sizeOfVertex() const;

        /**
         * @brief Return the number of vertices
         */
        size_t vertexCount() const;

        /**
         * @brief Merge and copy the vertex buffer content to the given buffer
         */
        void copyTo(std::vector<std::byte>& buffer) const;

        /**
         * @brief Insert a data in the given attribute array
         */
        template <typename Type>
        void insert(AttributeType type, const Type& value);

        /**
         * @brief Return true if the given attribute contains data
         */
        bool hasAttribute(AttributeType type) const;

        /**
         * @brief Return the locatino of the given attribute
         */
        int getLocation(AttributeType type) const;

        /**
         * @brief Return the number of component for the given attribue
         */
        size_t getComponentCount(AttributeType type) const;

    private:
        // Data
        std::array<std::vector<std::byte>, count> data;
        std::array<size_t, count> componentCount;
        std::array<size_t, count> attributeSize;
    };

    template <typename Type>
    void MeshVertexBuffer::insert(AttributeType type, const Type& value)
    {
        size_t size = data[type].size();
        data[type].resize(size + sizeof(value));
        std::memcpy(&data[type][size], &value, sizeof(value));
    }
}
