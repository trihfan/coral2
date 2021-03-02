#pragma once
#include <vector>

namespace backend
{
    struct BackendVertexBufferParams
    {
    };

    struct BackendVertexAttribute
    {
        int size;
    };

    struct BackendVertexBufferData
    {
        // Vertices
        void* vertices;
        int verticesCount;

        // Indices
        void* indices;
        int indicesCount;

        // Vertex description
        int vertexSize;
        std::vector<BackendVertexAttribute> vertexAttributes;
    };

    class BackendVertexBuffer
    {
    public:
        BackendVertexBuffer(const BackendVertexBufferParams& params, const BackendVertexBufferData& data);
        virtual ~BackendVertexBuffer() = default;

        virtual void draw() = 0;

    protected:
        const BackendVertexBufferParams params;
    };
}
