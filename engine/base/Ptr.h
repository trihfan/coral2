#pragma once

#include <memory>

namespace coral
{
    template <typename Type>
    using ptr = std::shared_ptr<Type>;

    template <typename Type>
    using wptr = std::weak_ptr<Type>;
}
