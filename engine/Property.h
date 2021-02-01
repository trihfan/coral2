#ifndef PROPERTY_H
#define PROPERTY_H

#include "Signal.h"

namespace coral
{
    // A property, contains a value and a signal on changed
    template<typename Type>
    struct Property
    {
        // Getters
        const Type& get() const;
        const Type& operator->() const;
        const Type& operator*() const;

        // Setters
        void set(const Type& value);
        const Property<Type>& operator=(const Type& value);

        // Signals
        Signal<const Type&> changed;

    protected:
        Type value;
    };

    // Extend Property for an array
    template<typename Type, typename ArrayType = std::vector<Type>>
    struct PropertyArray : public Property<ArrayType>
    {
        // Return the array size
        size_t size() const;

        // Get a value from the array
        const Type& get(size_t index) const;
        const Type& operator[](size_t index) const;

        // Set a value in the array
        void set(const Type& value, size_t index);

        // Append a value to the array
        void add(const Type& value);

        // Insert a value in the array
        void insert(const Type& value, size_t index);

        // Remove the value
        void remove(const Type& value);

        // Erase the value at the given index
        void erase(size_t index);

        // Return true if the value is in the array
        bool contains(const Type& value);
    };

    template<typename Type>
    const Type& Property<Type>::get() const
    {
        return value;
    }

    template<typename Type>
    void Property<Type>::set(const Type& value)
    {
        this->value = value;
        this->changed(this->value);
    }

    template<typename Type>
    const Property<Type>& Property<Type>::operator=(const Type& value)
    {
        set(value);
        return *this;
    }

    template<typename Type>
    const Type& Property<Type>::operator->() const 
    { 
        return value; 
    }

    template<typename Type>
    const Type& Property<Type>::operator*() const 
    { 
        return value; 
    }

    template<typename Type, typename ArrayType>
    size_t PropertyArray<Type, ArrayType>::size() const
    {
        return value.size();
    }

    template<typename Type, typename ArrayType>
    const Type& PropertyArray<Type, ArrayType>::get(size_t index) const
    {
        return value[index];
    }

    template<typename Type, typename ArrayType>
    const Type& PropertyArray<Type, ArrayType>::operator[](size_t index) const
    {
        return value[index];
    }

    template<typename Type, typename ArrayType>
    void PropertyArray<Type, ArrayType>::set(const Type& value, size_t index)
    {
        this->value[index] = value;
        this->changed(this->value);
    }

    template<typename Type, typename ArrayType>
    void PropertyArray<Type, ArrayType>::add(const Type& value)
    {
        this->value.push_back(value);
        this->changed(this->value);
    }

    template<typename Type, typename ArrayType>
    void PropertyArray<Type, ArrayType>::insert(const Type& value, size_t index)
    {
        this->value.insert(this->value.begin() + index, value);
        this->changed(this->value);
    }

    template<typename Type, typename ArrayType>
    void PropertyArray<Type, ArrayType>::remove(const Type& value)
    {
        auto it = std::find(this->value.begin(), this->value.end(), value);
        if (it != this->value.end())
        {
            this->value.erase(it);
        }
    }

    template<typename Type, typename ArrayType>
    void PropertyArray<Type, ArrayType>::erase(size_t index)
    {
        value.erase(value.begin() + index);
    }

    template<typename Type, typename ArrayType>
    bool PropertyArray<Type, ArrayType>::contains(const Type& value)
    {
        return std::find(this->value.begin(), this->value.end(), value) != this->value.end();
    }
}
#endif
