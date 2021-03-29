#pragma once

#include "base/Ptr.h"
#include <memory>
#include <string>

namespace coral
{
    class ObjectFactory;

    enum class ObjectState
    {
        not_initialized,
        initialized,
        released
    };

    // The Object class represent an object in coral engine
    class Object : public std::enable_shared_from_this<Object>
    {
        friend class ObjectFactory;

    public:
        // construction
        Object();
        virtual ~Object();

        // Handle
        template <typename Type>
        ptr<Type> toPtr();

        // type
        template <typename Type>
        bool isA() const;

        // State
        ObjectState getState() const;

        // name
        void setName(const std::string& name);
        const std::string& getName() const;

        // Initialize the resources
        virtual void init();

        // Release the resources
        virtual void release();

    private:
        std::string name;
        ObjectState state;
    };

    template <typename Type>
    ptr<Type> Object::toPtr()
    {
        return std::dynamic_pointer_cast<Type>(shared_from_this());
    }

    template <typename Type>
    bool Object::isA() const
    {
        return dynamic_cast<const Type*>(this) != nullptr;
    }
}
