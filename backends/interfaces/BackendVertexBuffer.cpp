#include "BackendVertexBuffer.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendVertexBuffer>(const BackendVertexBufferParams&, const BackendVertexBufferData&)> creator<BackendVertexBuffer, BackendVertexBufferParams, BackendVertexBufferData> = nullptr;

BackendVertexBuffer::BackendVertexBuffer(const BackendVertexBufferParams& params, const BackendVertexBufferData&)
    : params(params)
{
}
