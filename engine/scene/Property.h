#pragma once
#include "base/Signal.h"

namespace coral
{
    /**
     * A property embedded a value
     */
    template <typename Type>
    struct Property
    {
        // Construction
        Property() = default;
        Property(const Type& defaultValue);

        // Access
        void set(const Type& value);
        const Type& get() const;

        // Modified signal
        Signal<> modified;

    private:
        Type value;
    };

    template <typename Type>
    Property<Type>::Property(const Type& defaultValue)
        : value(defaultValue)
    {
    }

    template <typename Type>
    void Property<Type>::set(const Type& value)
    {
        if (this->value != value)
        {
            this->value = value;
            modified();
        }
    }

    template <typename Type>
    const Type& Property<Type>::get() const
    {
        return value;
    }
}
