#pragma once
#include "signals/Signal.h"

namespace coral
{
    /**
     * A property embedded a value
     */
    template <typename Type>
    class Property
    {
    public:
        // Construction
        Property() = default;
        Property(const Type& defaultValue);

        // Access
        void set(const Type& value);
        template <typename OtherType> void operator=(const OtherType& value) { set(value); }
        const Type& get() const;
        const Type* operator->() const;
        const Type& operator*() const;
        operator const Type&() const;

        // Modified signal
        Signal<> modified;

    private:
        Type value;
    };

    // Comparison
    template <typename Type> bool operator==(const Property<Type>& p, const Type& o) { return *p == o; }
    template <typename Type> bool operator==( const Type& o, const Property<Type>& p) { return o == *p; }
    template <typename Type> bool operator!=(const Property<Type>& p, const Type& o) { return *p != o; }
    template <typename Type> bool operator!=( const Type& o, const Property<Type>& p) { return o != *p; }

    // Implementation
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

    template <typename Type>
    const Type* Property<Type>::operator->() const
    {
        return &value;
    }

    template <typename Type>
    const Type& Property<Type>::operator*() const
    {
        return value;
    }

    template <typename Type>
    Property<Type>::operator const Type&() const
    {
        return value;
    }
}
