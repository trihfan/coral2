#include "OpenglVertexBuffer.h"
#include "Logs.h"
#include "OpenglError.h"
#include "gl.h"

using namespace coral;
using namespace backend::opengl;

OpenglVertexBuffer::OpenglVertexBuffer(const BackendVertexBufferData& data)
    : BackendVertexBuffer(data)
    , size(data.indicesCount)
{
    // Vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, data.verticesCount * data.vertexSize, data.vertices, GL_STATIC_DRAW);

    // Index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(data.indicesCount) * sizeof(unsigned int), data.indices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    int offset = 0;
    for (size_t i = 0; i < data.vertexAttributes.size(); i++)
    {
        if (data.vertexAttributes[i].location != -1)
        {
            glEnableVertexAttribArray(static_cast<GLuint>(data.vertexAttributes[i].location));
            glVertexAttribPointer(
                static_cast<GLuint>(data.vertexAttributes[i].location),
                data.vertexAttributes[i].size,
                GL_FLOAT,
                GL_FALSE,
                data.vertexSize,
                reinterpret_cast<void*>(static_cast<size_t>(offset) * sizeof(float)));
        }
        offset += data.vertexAttributes[i].size;
    }

    glBindVertexArray(0);
    CHECK_OPENGL_ERROR
}

OpenglVertexBuffer::~OpenglVertexBuffer()
{
}

void OpenglVertexBuffer::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(size), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    CHECK_OPENGL_ERROR
}
