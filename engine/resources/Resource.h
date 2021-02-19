#pragma once

#include <GL/glew.h>

#include "Object.h"
#include "Property.h"

namespace coral
{
    class Engine;

    //
    class Resource : public Object
    {
    public:
        Resource();

        void bind();
        GLuint getId() const;

        // Properties
        Property<int> sampleCount;
        Property<int> width;
        Property<int> height;
        Property<GLint> internalFormat;
        Property<GLenum> format;
        Property<GLenum> type;

    private:
        // initialization
        void init();
        void release();

    private:
        GLuint id;
    };
}
