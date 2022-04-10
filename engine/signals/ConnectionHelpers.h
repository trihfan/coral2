#pragma once
#include <functional>

namespace coral
{
    template <typename T> struct FunctionTraits : public FunctionTraits<decltype(&T::operator())> {};

    template <typename Class, typename Return, typename... Parameters>
    struct FunctionTraits<Return(Class::*)(Parameters...) const>
    {
        using result = Return;
        using parameters = std::tuple<Parameters...>;
        static constexpr auto arity = sizeof...(Parameters);
    };

    template <typename... Args> struct LambdaContainer
    {
        std::function<void(Args...)> lambda;
        inline void operator()(Args... args) { lambda(std::forward<Args>(args)...); }
    };
}
