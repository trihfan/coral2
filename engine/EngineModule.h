#include "utils/Singleton.h"

namespace coral
{
    template <typename Type>
    class EngineModule : public Singleton<Type>
    {
    public:  
        // Engine module methods
        virtual void init() {}  
        virtual void release() {}
        virtual void update() {}  
    };
}