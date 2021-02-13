#ifndef OBJECT_H
#define OBJECT_H

#include "Signal.h"
#include <memory>
#include <string>

namespace coral
{
    class Engine;

    // The Object class represent an object in coral engine
    class Object : public std::enable_shared_from_this<Object>
    {
        friend class ObjectManager;

    public:
        // type
        template <typename Type>
        bool isA() const { return dynamic_cast<const Type*>(this) != nullptr; }

        // name
        void setName(const std::string& name);
        const std::string& getName() const;

        // handle
        template <typename ObjectType>
        inline std::shared_ptr<ObjectType> toHandle() { return std::dynamic_pointer_cast<ObjectType>(shared_from_this()); }

        template <typename ObjectType>
        inline std::shared_ptr<const ObjectType> toHandle() const { return std::dynamic_pointer_cast<const ObjectType>(shared_from_this()); }

        // Signals
        Signal<> init;
        Signal<> release;
        Signal<> update;

    protected:
        // construction
        Object();
        virtual ~Object();

        // state
        enum class ObjectState
        {
            not_initialized,
            initialized,
            released
        } state;
        ObjectState getState() const;

    private:
        std::string name;
    };
}
#endif
