#pragma once
#include "Property.h"
#include "Engine.h"
#include "ObjectManager.h"
#include <string>

namespace coral
{
    class ObjectInterface;

    /**
     * @brief The Object class is a handle for engine object
     * an engine object must extend ObjectInterface fo initialization and release the resources are
     * handledby the engine
     */
    template <typename Type> class Object
    {
    public:
        static_assert(std::is_base_of<ObjectInterface, Type>::value);

        // Creation
        template <class... Args> static Object<Type> create(Args&&... args)
        {
            Object<Type> object = std::make_shared<Type>(std::forward<Args>(args)...);
            engine->objectManager->add(object);
            return object;
        }
        template <class... Args> static Object<Type> createWithName(const std::string& name, Args&&... args)
        {
            auto object = create(std::forward<Args>(args)...);
            object->name = name;
            return object;
        }

        // Null handle
        Object() = default;
        Object(std::nullptr_t) : Object() {}

        // Copy
        template <typename OtherType> Object(const Object<OtherType>& other) : ptr(other.ptr) {}
        template <typename OtherType> Object(Object<OtherType>&& other) : ptr(std::move(other.ptr)) {}

        // Destruction
        ~Object() { bool checkIfUnique = ptr != nullptr; ptr = nullptr; if (checkIfUnique && unique()) { engine->objectManager->remove(*this); } }

        // Assignement
        template <typename OtherType> Object<OtherType>& operator=(const Object<OtherType>& other) { ptr = other.other; return *this; }

        // Cast
        template <typename OtherType> Object<OtherType> to() { return Object<OtherType>(std::dynamic_pointer_cast<OtherType>(ptr)); }

        // Conversion
        operator bool() const { return ptr != nullptr; }
        operator Type&() { return *ptr; }
        operator const Type&() const { return *ptr; }

        // Access
        Type* operator->() { return ptr.get(); }
        const Type* operator->() const { return ptr.get(); }
        Type& operator*() { return *ptr; }
        const Type& operator*() const { return *ptr; }

        // Status
        uint32_t useCount() const { return static_cast<uint32_t>(ptr.use_count()); }
        bool unique() const { return useCount() == 1;}
        bool expired() const { return useCount() == 0; }

        // Reset the handle -> ask for re-init
        void reset()
        {
            engine->objectManager->reset(*this);
        }

    private:
        std::shared_ptr<Type> ptr;

        // Constructor from shared_ptr
        Object(const std::shared_ptr<Type>& ptr) : ptr(ptr) {}

        // Add friend class to construct Object from shared_ptr of ObjectInterface
        friend class ObjectInterface;
    };

    // Comparison
    template <class T, class U> bool operator==(const Object<T>& lhs, const Object<U>& rhs) noexcept { return lhs.ptr == rhs.ptr; }
    template <class T> bool operator==(const Object<T>& lhs, std::nullptr_t) noexcept { return lhs.ptr == nullptr; }
    template <class T> bool operator==(std::nullptr_t, const Object<T>& rhs) noexcept { return nullptr == rhs.ptr; }
    template <class T> bool operator!=(const Object<T>& lhs, std::nullptr_t) noexcept { return lhs.ptr != nullptr; }
    template <class T> bool operator!=(std::nullptr_t, const Object<T>& rhs) noexcept { return nullptr != rhs.ptr; }
    template <class T, class U> bool operator<(const Object<T>& lhs, const Object<U>& rhs) noexcept { return lhs.ptr < rhs.ptr; }
    template <class T> bool operator<(const Object<T>& lhs, std::nullptr_t) noexcept { return lhs.ptr < nullptr; }
    template <class T> bool operator<(std::nullptr_t, const Object<T>& rhs) noexcept { return nullptr < rhs.ptr; }
    template <class T> bool operator<=(const Object<T>& lhs, std::nullptr_t) noexcept { return lhs.ptr <= nullptr; }
    template <class T> bool operator<=(std::nullptr_t, const Object<T>& rhs) noexcept { return nullptr <= rhs.ptr; }
    template <class T> bool operator>(const Object<T>& lhs, std::nullptr_t) noexcept { return lhs.ptr > nullptr; }
    template <class T> bool operator>(std::nullptr_t, const Object<T>& rhs) noexcept { return nullptr > rhs.ptr; }
    template <class T> bool operator>=(const Object<T>& lhs, std::nullptr_t) noexcept { return lhs.ptr >= nullptr; }
    template <class T> bool operator>=(std::nullptr_t, const Object<T>& rhs) noexcept { return nullptr >= rhs.ptr; }
    template <class T> bool operator!(const Object<T>& o) noexcept { return o == nullptr; }

    /**
     * @brief The ObjectRef class provide a ref to an object, it acts as a weak ptr and so must
     * be locked before use
     */
    template <typename Type>
    class ObjectRef
    {
    public:
        static_assert(std::is_base_of<ObjectInterface, Type>::value);

        // Copy and null handle
        ObjectRef() = default;
        ObjectRef(std::nullptr_t) : ObjectRef() {}
        template <typename OtherType> ObjectRef(const Object<OtherType>& other) : ptr(other.ptr) {}
        template <typename OtherType> ObjectRef(Object<OtherType>&& other) : ptr(std::move(other.ptr)) {}
        template <typename OtherType> ObjectRef(const ObjectRef<OtherType>& other) : ptr(other.ptr) {}

        // Assignement
        template <typename OtherType> ObjectRef<OtherType>& operator=(const ObjectRef<OtherType>& other) { ptr = other.other; return *this; }
        template <typename OtherType> ObjectRef<OtherType>& operator=(const Object<OtherType>& other) { ptr = other.other; return *this; }

        // Status
        uint32_t useCount() const { return static_cast<uint32_t>(ptr.use_count()); }
        bool unique() const { return useCount() == 1;}
        bool expired() const { return useCount() == 0; }

        // Lock and return an Object from the ref, if the ref is already deleted the Object will be null
        Object<Type> lock() const { return Object<Type>(ptr.lock()); }

    private:
        std::weak_ptr<Type> ptr;
    };
}
