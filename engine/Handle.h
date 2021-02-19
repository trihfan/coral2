#pragma once

#include <atomic>
#include <string>

namespace coral
{
    class ObjectFactory;
    /**
     * @brief Shared memory for Handle
     */
    struct HandleSharedMemory
    {
        std::atomic<uint32_t> useCount = 0;
        uint64_t index;
    };

    /**
     * @brief Handle to manage engine objects
     */
    template <typename ObjectType>
    class Handle
    {
    public:
        // Construction
        Handle();
        Handle(std::nullptr_t null);
        Handle(ObjectType* data, HandleSharedMemory* sharedMemory);
        Handle(const Handle& other);
        Handle(Handle&& other);
        ~Handle();

        // Assignement
        Handle& operator=(const Handle& other);

        // Access
        ObjectType* operator*() const;
        ObjectType* operator->() const;

        // Conversion
        template <typename Other>
        operator Handle<Other>() const;
        operator bool() const;

        // Comparison
        bool operator==(const Handle& other) const;
        bool operator!=(const Handle& other) const;
        bool operator<(const Handle& other) const;
        bool operator<=(const Handle& other) const;
        bool operator>(const Handle& other) const;
        bool operator>=(const Handle& other) const;

        // Meta
        uint32_t useCount() const;

    private:
        friend ObjectFactory;
        ObjectType* data;
        HandleSharedMemory* sharedMemory;

        void set(ObjectType* data, HandleSharedMemory* meta);
        void destroy();
    };

    /**
     * @brief The class HandleFromThis allow any class deriving from it to call toHandle
     * to obtain an handle from the class ptr
     */
    class HandleFromThis
    {
    public:
        // Destructor
        virtual ~HandleFromThis() = default;

        /**
         * @brief Return an handle of type ObjectType for this object
         */
        template <typename ObjectType>
        inline Handle<ObjectType> toHandle() const;

    private:
        template <typename ObjectType>
        friend class Handle;
        HandleSharedMemory* sharedMemoryPtr = nullptr;
    };
}

#include "HandleImpl.h"
