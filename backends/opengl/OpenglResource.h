#pragma once

#include "BackendResource.h"
#include "gl.h"

namespace backend::opengl
{
    /**
     * @brief The OpenglResource class
     */
    class OpenglResource : public BackendResource
    {
    public:
        OpenglResource(const BackendResourceParams& params);
        ~OpenglResource() override;
        virtual void bind(int index) override;

        GLuint getId() const;
        int getSamplesCount() const;

    private:
        GLuint id;
        int samples;

        static GLint getInternalFormatFrom(BackendResourceFormat format);
        static GLenum getFormatFrom(BackendResourceFormat format);
        static GLenum getTypeFrom(BackendResourceFormat format);
    };
}
