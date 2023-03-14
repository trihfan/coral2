#pragma once
#include "Property.h"
#include <string>

namespace coral
{
    class ObjectManager;
    enum class InitState { notInitialized, initialized, released };

    /**
     * @brief An object is an engine object providing an init() and release() method to manage gpu data
     */
    class Object : private std::enable_shared_from_this<Object>
    {
        friend class ObjectManager;
    public:
        // Creation
        virtual ~Object();

        // meta
        template <typename Type> bool isA() const { return dynamic_cast<const Type*>(this) != nullptr; }

        // Properties
        Property<std::string> name;

        // The Object can't be copied
        Object(const Object&) = delete;
        Object(Object&&) = delete;
        Object& operator=(const Object&) = delete;
    protected:
        // Initialization methods
        virtual void init() {}
        virtual void release() {}

        // Discard current initialized data, this will cause a call to release() followed by a call to init()
        void reset();

        // Construction
        Object();

    private:
        // Meta
        InitState state;
        bool willReset;
    };
}
