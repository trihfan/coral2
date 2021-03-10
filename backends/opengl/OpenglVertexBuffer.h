#pragma once
#include "BackendVertexBuffer.h"
#include "gl.h"

namespace backend::opengl
{
    class OpenglVertexBuffer : public BackendVertexBuffer
    {
    public:
        OpenglVertexBuffer(const BackendVertexBufferData& data);
        ~OpenglVertexBuffer() override;
        void draw() override;

    protected:
        GLuint vao;
        GLuint vertexBuffer;
        GLuint indexBuffer;
        int size;
    };
}
