#pragma once
#include "BackendPipeline.h"
#include <vector>

namespace backend
{
    enum BackendVertexAttributeType
    {
        int32,
        float32
    };

    struct BackendVertexAttribute
    {
        int location;
        int size;
        BackendVertexAttributeType type;
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
        BackendVertexBuffer(const BackendVertexBufferData& data);
        virtual ~BackendVertexBuffer() = default;
        virtual void draw() = 0;
    };
}
