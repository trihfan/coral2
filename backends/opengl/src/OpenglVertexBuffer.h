#pragma once
#include "BackendVertexBuffer.h"
#include "glad/glad.h"

namespace backend::opengl
{
    class OpenglVertexBuffer : public BackendVertexBuffer
    {
    public:
        OpenglVertexBuffer(const BackendVertexBufferParams& params, const BackendVertexBufferData& data);
        ~OpenglVertexBuffer() override;
        void draw() override;

    protected:
        GLuint vao;
        GLuint vertexBuffer;
        GLuint indexBuffer;
        int size;
    };
}
