#pragma once
#include "resources/VertexBuffer.h"
#include "scene/DrawableNode.h"
#include <vector>

namespace coral
{
    /**
     * @brief A mesh drawable node using vertices and indices
     */
    class Mesh : public DrawableNode
    {
    public:
        // constructor
        Mesh();

        // Properties
        Property<VertexBuffer> vertexBuffer;

        // render the mesh
        virtual void init() override;
        virtual void release() override;
        virtual void draw() override;

    private:
        // Data holders
        std::vector<unsigned int> indices;

        // initializes all the buffer objects/arrays
        //void setupMesh();

        //std::vector<backend::BackendVertexAttribute> createAttributeArray() const;
    };
}
