#pragma once
#include "ConnectionHelpers.h"

namespace coral
{
    /**
     * @brief The type of connection
     */
    enum class ConnectionType
    {
        method, function, lambda
    };

    // Declaration
    template <typename... Args> class Signal;
    template <typename... Args> using Function = void (*)(Args...);

    /**
     * @brief The Connection class
     */
    template <typename... Args> class Connection
    {
    public:
        // Function constructor
        Connection(const Signal<Args...>& signal, Function<Args...> function) : signal(signal), type(ConnectionType::function), function(reinterpret_cast<void*>(function)) { }

        // Methods constructor
        template <typename Method, typename Object>
        Connection(const Signal<Args...>& signal, Method method, Object* object) : signal(signal), type(ConnectionType::method), object(reinterpret_cast<void*>(object)), function(*reinterpret_cast<void**>(&method)) { }

        // Lambda constructor
        template <typename Lambda, typename std::enable_if<std::is_convertible<Lambda, std::function<void(Args...)>>::value, bool>::type = true>
        Connection(const Signal<Args...>& signal, Lambda&& lambda) : signal(signal), type(ConnectionType::lambda), function(reinterpret_cast<void*>(new LambdaContainer<Args...>{ lambda })) { }

        // Lambda with no argument constructor for simlpification
        template <typename Lambda, typename std::enable_if<std::is_convertible<Lambda, std::function<void()>>::value && !std::is_same<std::function<void()>, std::function<void(Args...)>>::value, bool>::type = true>
        Connection(const Signal<Args...>& signal, Lambda&& lambda) : signal(signal), type(ConnectionType::lambda), function(reinterpret_cast<void*>(new LambdaContainer<Args...>{[lambda](Args...){ lambda(); }})) { }

        // Destructor
        ~Connection()
        {
            if (type == ConnectionType::lambda)
            {
                delete reinterpret_cast<LambdaContainer<Args...>*>(function);
            }
        }

        void disconnect() { signal.disconnect(this); }

        // Call the connection
        void operator()(Args... args) const
        {
            switch (type)
            {
            case ConnectionType::method:
                reinterpret_cast<void (*)(void*, Args...)>(function)(object, std::forward<Args>(args)...);
                break;
            case ConnectionType::function:
                reinterpret_cast<Function<Args...>>(function)(std::forward<Args>(args)...);
                break;
            case ConnectionType::lambda:
                std::cout << "lambda" << std::endl;
                (*reinterpret_cast<LambdaContainer<Args...>*>(function))(std::forward<Args>(args)...);
                break;
            }
        }

    private:
        const Signal<Args...>& signal;
        ConnectionType type;
        mutable void* object;
        mutable void* function;
    };
}
