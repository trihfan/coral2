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
            boneId,
            boneWeight,
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
         * @brief Merge and copy the vertex buffer content to the a buffer
         */
        std::vector<std::byte> pack();

        // Add data
        void addPosition(const glm::vec3& position);
        void addNormal(const glm::vec3& normal);
        void addTextCoord(const glm::vec2& textCoords);
        void addBoneIncidence(size_t verticeIndex, int id, float weight);

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
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<std::vector<std::pair<int, float>>> boneIncidences;
        std::vector<std::pair<glm::vec4, glm::vec4>> packedBoneIncidences;

        /**
         * @brief Pack boneIncidences inside packedBoneIncidences
         */
        void packBoneIncidences();

        /**
         * @brief Return a ptr to the value of the given index for the given attribute
         */
        const void* getPtrTo(AttributeType type, size_t index) const;
    };
}
