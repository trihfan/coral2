#include "BackendVertexBuffer.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendVertexBuffer>(const BackendVertexBufferData&)> creator<BackendVertexBuffer, BackendVertexBufferData> = nullptr;

BackendVertexBuffer::BackendVertexBuffer(const BackendVertexBufferData& data)
    : data(data)
{
}
