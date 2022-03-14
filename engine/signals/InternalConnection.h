#pragma once

#include <functional>
#include <memory>

namespace coral
{
    template <typename... Args>
    class InternalConnectionMethod
    {
    public:
        InternalConnectionMethod(void* object, void* method);
        void operator()(Args... args) const;

    public:
        mutable void* object;
        mutable void* method;
    };

    template <typename... Args>
    class InternalConnectionFunction
    {
    public:
        InternalConnectionFunction(void* function);
        void operator()(Args... args) const;

    public:
        mutable void* function;
    };

    template <typename... Args>
    class InternalConnectionLambda
    {
    public:
        InternalConnectionLambda(std::function<void(Args...)>&& lambda);
        void operator()(Args... args) const;

    public:
        mutable std::function<void(Args&&...)> lambda;
    };

    // -- Implementation --
    template <typename... Args>
    InternalConnectionMethod<Args...>::InternalConnectionMethod(void* object, void* method)
        : object(object)
        , method(method)
    {
    }

    template <typename... Args>
    void InternalConnectionMethod<Args...>::operator()(Args... args) const
    {
        reinterpret_cast<void (*)(void*, Args...)>(method)(&object, std::forward<Args>(args)...);
    }

    template <typename... Args>
    InternalConnectionFunction<Args...>::InternalConnectionFunction(void* function)
        : function(function)
    {
    }

    template <typename... Args>
    void InternalConnectionFunction<Args...>::operator()(Args... args) const
    {
        reinterpret_cast<void (*)(Args...)>(function)(std::forward<Args>(args)...);
    }

    template <typename... Args>
    InternalConnectionLambda<Args...>::InternalConnectionLambda(std::function<void(Args...)>&& lambda)
        : lambda(lambda)
    {
    }

    template <typename... Args>
    void InternalConnectionLambda<Args...>::operator()(Args... args) const
    {
        lambda(std::forward<Args>(args)...);
    }
}
