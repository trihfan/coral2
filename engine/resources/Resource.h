#ifndef RESOURCE_H
#define RESOURCE_H

#include "Object.h"
#include "Property.h"
#include "glad/glad.h"

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
#endif
