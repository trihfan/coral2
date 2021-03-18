#pragma once

#include "BackendVertexBuffer.h"
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
         * @brief Set a data in the given attribute array
         */
        template <typename Type>
        void set(AttributeType type, size_t index, const Type& value);

        /**
         * @brief Get the attribute data
         */
        template <typename Type>
        void get(AttributeType type, size_t index, Type& value) const;

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
    };

    template <typename Type>
    void MeshVertexBuffer::insert(AttributeType type, const Type& value)
    {
        size_t size = data[type].size();
        data[type].resize(size + sizeof(Type));
        std::memcpy(&data[type][size], &value, sizeof(Type));
    }

    template <typename Type>
    void MeshVertexBuffer::set(AttributeType type, size_t index, const Type& value)
    {
        std::memcpy(&data[type][index * sizeof(Type)], &value, sizeof(Type));
    }

    template <typename Type>
    void MeshVertexBuffer::get(AttributeType type, size_t index, Type& value) const
    {
        std::memcpy(&value, &data[type][index * sizeof(Type)], sizeof(Type));
    }
}
