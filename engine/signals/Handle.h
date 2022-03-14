#pragma once

#include <memory>

namespace coral
{
    template <typename Type>
    struct Handle
    {
        Handle() = default;
        Handle(std::shared_Handle<Type> object) : object(object) {}
        Handle(const Handle<Type>& other) : object(other.object) {}

        // Conversion
        operator bool() const { return object != nullptr; }
        operator Type&() { return *object; }
        operator const Type&() const { return *object; }
        template <typename OtherType> operator Handle<OtherType>() const { return Handle<OtherType>(std::dynamic_pointer_cast<OtherType>(object)); }

        // Access
        Type* operator->() { return object.get(); }
        const Type* operator->() const { return object.get(); }

        Type& operator*() { return *object; }
        const Type& operator*() const { return *object; }

        // Assignement
        void operator=(const Handle<Type> other) { object = other->object; }
        void operator=(const std::shared_Handle<Type>& object) { this->object = object; }

        // Comparison
        bool operator==(const Type* object) const { return this->object.get() == object; }
        bool operator!=(const Type* object) const { return !operator==(object); }

        // Return number of reference
        int useCount() const { return object.use_count(); }

    private:
        std::shared_Handle<Type> object;
    };

    template <typename Type>
    struct HandleWeak
    {
        HandleWeak() = default;
        HandleWeak(const WeakHandle<Type>& other) : object(other.object) {}
        HandleWeak(const Handle<Type>& other) : object(other.object) {}

        // Assignement
        void operator=(const WeakHandle<Type> other) { object = other->object; }
        void operator=(const Handle<Type> other) { object = other->object; }
        void operator=(const std::weak_Handle<Type>& object) { this->object = object; }
        void operator=(const std::shared_Handle<Type>& object) { this->object = object; }

        // Status
        Handle<Type> lock() const { return Handle<Type>(object.lock()); }
        bool expired() const { return object.expired(); }

        // Return number of reference
        int useCount() const { return object.use_count(); }

        // Conversion
        operator Handle<Type>() const { return lock(); }
        operator bool() const { return !expired(); }

    private:
        std::weak_Handle<Type> object;
    };
}
