#include "Mesh.h"
#include "utils/Logs.h"
#include "materials/Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace coral;

Mesh::Mesh(const VertexBuffer& vertexBuffer, const std::vector<unsigned int>& indices)
    : vertexBuffer(vertexBuffer)
    , indices(indices)
{
    //addRenderQueueTag(defaultRenderPassName);
    connect<&Node::reset>(this->vertexBuffer.modified, this);
}

void Mesh::draw()
{
    // Draw
    //vertexBuffer.draw();
}

void Mesh::init()
{
    DrawableNode::init();

    // Data
    /*backend::BackendVertexBufferData data;

    // Copy vertices
    data.verticesCount = static_cast<int>(vertices.vertexCount());
    std::vector<std::byte> vertexData = vertices.pack();
    data.vertices = vertexData.data();

    // Copy indices
    data.indicesCount = static_cast<int>(indices.size());
    data.indices = indices.data();

    // Attributes
    data.vertexSize = static_cast<int>(vertices.sizeOfVertex());
    data.vertexAttributes = createAttributeArray();

    // Create buffer
    vertexBuffer = backend::BackendObjectFactory<backend::BackendVertexBuffer>::create(data);*/
}

/*std::vector<backend::BackendVertexAttribute> Mesh::createAttributeArray() const
{
    std::vector<backend::BackendVertexAttribute> attributes;
    if (!material)
    {
        return attributes;
    }

    for (size_t i = 0; i < MeshVertexBuffer::count; i++)
    {
        auto type = static_cast<MeshVertexBuffer::AttributeType>(i);
        if (vertices.hasAttribute(type))
        {
            backend::BackendVertexAttribute attribute;
            attribute.size = static_cast<int>(vertices.getComponentCount(type));
            attribute.location = static_cast<int>(i);
            attributes.push_back(attribute);
        }
    }
    return attributes;
}*/

void Mesh::release()
{
    DrawableNode::release();
    //vertexBuffer = nullptr;
}
