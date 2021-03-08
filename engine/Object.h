#pragma once

#include "Handle.h"
#include "Signal.h"
#include <memory>
#include <string>
#include <string_view>

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
    class Object : public HandleFromThis
    {
        friend class ObjectFactory;

    public:
        // construction
        Object();
        virtual ~Object() override;

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

        // Update the Object, called each frame
        virtual void update();

    protected:
        // state
        ObjectState state;

    private:
        std::string name;
    };

    template <typename Type>
    bool Object::isA() const
    {
        return dynamic_cast<const Type*>(this) != nullptr;
    }
}
