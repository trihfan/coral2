#pragma once

#include "MemoryResource.h"
#include "ObjectFactoryData.h"
#include "utils/Logs.h"

namespace coral
{
    template <typename ObjectType>
    Handle<ObjectType>::Handle()
        : data(nullptr)
        , sharedMemory(nullptr)
    {
    }

    template <typename ObjectType>
    Handle<ObjectType>::Handle(std::nullptr_t null)
        : data(nullptr)
        , sharedMemory(nullptr)
    {
    }

    template <typename ObjectType>
    Handle<ObjectType>::Handle(ObjectType* data, HandleSharedMemory* sharedMemory)
        : data(nullptr)
        , sharedMemory(nullptr)
    {
        set(data, sharedMemory);
    }

    template <typename ObjectType>
    Handle<ObjectType>::Handle(const Handle& other)
        : data(nullptr)
        , sharedMemory(nullptr)
    {
        set(other.data, other.sharedMemory);
    }

    template <typename ObjectType>
    Handle<ObjectType>::Handle(Handle&& other)
        : data(nullptr)
        , sharedMemory(nullptr)
    {
        set(other.data, other.sharedMemory);
        other.set(nullptr, nullptr);
    }

    template <typename ObjectType>
    Handle<ObjectType>::~Handle()
    {
        set(nullptr, nullptr);
    }

    template <typename ObjectType>
    template <typename Other>
    Handle<ObjectType>::operator Handle<Other>() const
    {
        return Handle<Other>(dynamic_cast<Other*>(data), sharedMemory);
    }

    template <typename ObjectType>
    Handle<ObjectType>::operator bool() const
    {
        return data != nullptr;
    }

    template <typename ObjectType>
    Handle<ObjectType>& Handle<ObjectType>::operator=(const Handle& other)
    {
        set(other.data, other.sharedMemory);
        return *this;
    }

    template <typename ObjectType>
    ObjectType* Handle<ObjectType>::operator*() const
    {
        return data;
    }

    template <typename ObjectType>
    ObjectType* Handle<ObjectType>::operator->() const
    {
        return data;
    }

    template <typename ObjectType>
    bool Handle<ObjectType>::operator==(const Handle& other) const
    {
        return data == other.data;
    }

    template <typename ObjectType>
    bool Handle<ObjectType>::operator!=(const Handle& other) const
    {
        return data != other.data;
    }

    template <typename ObjectType>
    bool Handle<ObjectType>::operator<(const Handle& other) const
    {
        return data < other.data;
    }

    template <typename ObjectType>
    bool Handle<ObjectType>::operator<=(const Handle& other) const
    {
        return data <= other.data;
    }

    template <typename ObjectType>
    bool Handle<ObjectType>::operator>(const Handle& other) const
    {
        return data > other.data;
    }

    template <typename ObjectType>
    bool Handle<ObjectType>::operator>=(const Handle& other) const
    {
        return data >= other.data;
    }

    template <typename ObjectType>
    void Handle<ObjectType>::set(ObjectType* data, HandleSharedMemory* sharedMemory)
    {
        // Remove old ref
        if (this->sharedMemory && !ObjectFactoryData::isDestroyed)
        {
            uint32_t count = this->sharedMemory->useCount.fetch_sub(1, std::memory_order_acquire);
            if (count == 2)
            {
                ObjectFactoryData::get()->releaseList.enqueue(*this);
            }
        }

        // Set new ref
        this->data = data;
        this->sharedMemory = sharedMemory;
        if (this->sharedMemory)
        {
            auto handleFromThis = dynamic_cast<HandleFromThis*>(data);
            if (handleFromThis && handleFromThis->sharedMemoryPtr != sharedMemory)
            {
                handleFromThis->sharedMemoryPtr = sharedMemory;
            }
            this->sharedMemory->useCount.fetch_add(1, std::memory_order_release);
        }
    }

    template <typename ObjectType>
    uint32_t Handle<ObjectType>::useCount() const
    {
        return sharedMemory->useCount.load(std::memory_order_relaxed);
    }

    template <typename ObjectType>
    inline Handle<ObjectType> HandleFromThis::toHandle() const
    {
        if (sharedMemoryPtr)
        {
            return Handle<ObjectType>(dynamic_cast<ObjectType*>(const_cast<HandleFromThis*>(this)), sharedMemoryPtr);
        }

        Logs(error) << "toHandle() called on not handled object";
        return nullptr;
    }
}
