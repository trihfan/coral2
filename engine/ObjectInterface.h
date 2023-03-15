#pragma once
#include "Property.h"
#include <string>

namespace coral
{
    class ObjectManager;
    template <typename Type> class Object;

    enum class InitState { notInitialized, initialized, released };

    /**
     * @brief An object is an engine object providing an init() and release() method to manage gpu data
     */
    class ObjectInterface : private std::enable_shared_from_this<ObjectInterface>
    {
        friend class ObjectManager;
    public:
        // Creation
        virtual ~ObjectInterface();

        // meta
        template <typename Type> Object<Type> getHandle() { return std::dynamic_pointer_cast<Type>(shared_from_this()); }
        template <typename Type> bool isA() const { return dynamic_cast<const Type*>(this) != nullptr; }

        // Properties
        Property<std::string> name;

        // The ObjectInterface can't be copied
        ObjectInterface(const ObjectInterface&) = delete;
        ObjectInterface(ObjectInterface&&) = delete;
        ObjectInterface& operator=(const ObjectInterface&) = delete;
    protected:
        // Initialization methods
        virtual void init() {}
        virtual void release() {}

        // Discard current initialized data, this will cause a call to release() followed by a call to init()
        void reset();

        // Construction
        ObjectInterface();

    private:
        // Meta
        InitState state;
        bool willReset;
    };
}
