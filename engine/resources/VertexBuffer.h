#pragma once
#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace coral
{
    // List of acceptable vertex buffer attributes
    enum AttributeType : size_t
    {
        position,
        normal,
        textCoord,
        boneId,
        boneWeight,
        count
    };

    struct VexterBufferParams
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<std::vector<std::pair<int, float>>> boneIncidences;
        std::vector<std::pair<glm::vec4, glm::vec4>> packedBoneIncidences;
    };

    /**
     * @brief The VertexBuffer struct contains data for a vertex buffer
     */
    class VertexBuffer
    {
    public:
        // Constructor
        VertexBuffer();
        VertexBuffer(const VexterBufferParams& params);
        void operator=(const VexterBufferParams& params);

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

    struct VertexBufferAttribute
    {
        int location;
        int size;
    };

    struct VertexBufferData
    {
        // Vertices
        void* vertices;
        int verticesCount;

        // Indices
        void* indices;
        int indicesCount;

        // Vertex description
        int vertexSize;
        std::vector<VertexBufferAttribute> vertexAttributes;
    };
}
