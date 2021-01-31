#ifndef PROPERTY_H
#define PROPERTY_H

#include "Signal.h"

namespace coral
{
    template<typename Type>
    struct Property
    {
        // Getter - setter
        const Type& get() const
        {
            return value;
        }

        void set(const Type& value)
        {
            this->value = value;
            changed.emit(this->value);
        }

        // Signals
        Signal<const Type&> changed;

    protected:
        Type value;
    };

    template<typename Type>
    struct PropertyArray : public Property<Type>
    {
        template<typename Value>
        void set(const Value& value, size_t index)
        {
            this->value[index] = value;
            this->changed.emit(this->value);
        }

        template<typename Value>
        void push_back(const Value& value)
        {
            this->value.push_back(value);
            this->changed.emit(this->value);
        }

        template<typename Value>
        void insert(const Value& value, size_t index)
        {
            this->value.insert(this->value.begin() + index, value);
            this->changed.emit(this->value);
        }
    };
}
#endif
