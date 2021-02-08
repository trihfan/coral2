#include <new>
#include "MemoryResource.h"

using namespace coral;

void* DefaultNewDeleteMemoryResource::do_allocate(size_t bytes, size_t alignment)
{
    return ::operator new(bytes, std::align_val_t(alignment));
}

void DefaultNewDeleteMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment)
{
    ::operator delete(p, bytes, std::align_val_t(alignment));
}

bool DefaultNewDeleteMemoryResource::do_is_equal(const memory_resource& other) const noexcept
{
    return this == &other;
}