#ifndef MEMORYRESOURCE_H
#define MEMORYRESOURCE_H

#include "memory_resource.h"

namespace coral
{
    // default memory resource using new and delete
    class DefaultNewDeleteMemoryResource : public std::pmr::memory_resource
    {
    protected:
        void* do_allocate(size_t bytes, size_t alignment) override;
        void do_deallocate(void* p, size_t bytes, size_t alignment) override;
        bool do_is_equal(const memory_resource& other) const noexcept override;
    };
}
#endif
