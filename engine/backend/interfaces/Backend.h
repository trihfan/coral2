#ifndef BACKEND_H
#define BACKEND_H

namespace coral
{
    class Backend
    {
    public:
        Backend() = default;
        virtual ~Backend() = default;

        virtual bool init() = 0;
        virtual bool destroy() = 0;
    };
}

#endif