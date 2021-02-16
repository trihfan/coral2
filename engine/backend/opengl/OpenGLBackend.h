#ifndef OPENGLBACKEND_H
#define OPENGLBACKEND_H

#include "backend/interfaces/Backend.h"

namespace coral
{
    class OpenGLBackend : public Backend
    {
    public:
        bool init() override;
        bool destroy() override;
    };
}

#endif