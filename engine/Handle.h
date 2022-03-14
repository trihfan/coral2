#pragma once

#include <memory>

namespace coral
{
    template <typename Type>
    using Handle = std::shared_ptr<Type>;

    template <typename Type>
    using WeakHandle = std::weak_ptr<Type>;
}
