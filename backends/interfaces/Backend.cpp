#pragma once

namespace coral
{
    class Backend
    {
    public:
        Backend() = default;
        virtual ~Backend() = default;

        virtual bool init() = 0;
        virtual bool destroy() = 0;
        virtual bool resize(int width, int height) = 0;
    };
}
