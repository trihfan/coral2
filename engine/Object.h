#pragma once

#include "Handle.h"
#include "Signal.h"
#include <memory>
#include <string>

namespace coral
{
    class ObjectFactory;

    // The Object class represent an object in coral engine
    class Object : public HandleFromThis
    {
        friend class ObjectFactory;

    public:
        // construction
        Object();
        virtual ~Object() override;

        // type
        template <typename Type>
        bool isA() const { return dynamic_cast<const Type*>(this) != nullptr; }

        // name
        void setName(const std::string& name);
        const std::string& getName() const;

        // Signals
        Signal<> init;
        Signal<> release;
        Signal<> update;

    protected:
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
