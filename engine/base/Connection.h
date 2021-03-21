#pragma once

#include "InternalConnection.h"
#include <memory>

namespace coral
{
    // Declaration
    template <typename... Args>
    class Signal;

    /**
     * @brief The Connection handle a connection to a signal
     */
    template <typename... Args>
    class Connection
    {
    public:
        /**
         * @brief Constructor with the signal and bind method
         */
        Connection(const Signal<Args...>& signal, std::unique_ptr<InternalConnectionMethod<Args...>> internalConnection);

        /**
         * @brief Constructor with the signal and bind function
         */
        Connection(const Signal<Args...>& signal, std::unique_ptr<InternalConnectionFunction<Args...>> internalConnection);

        /**
         * @brief Constructor with the signal and bind lambda
         */
        Connection(const Signal<Args...>& signal, std::unique_ptr<InternalConnectionLambda<Args...>> internalConnection);

        /**
         * @brief Call the bind method
         */
        void operator()(Args... args) const;

        /**
         * @brief Disconnect the connection from the signal
         */
        void disconnect();

    private:
        // The signal
        const Signal<Args...>& signal;

        // Actual connections
        std::unique_ptr<InternalConnectionMethod<Args...>> internalConnectionMethod;
        std::unique_ptr<InternalConnectionFunction<Args...>> internalConnectionFunction;
        std::unique_ptr<InternalConnectionLambda<Args...>> internalConnectionLambda;
    };

    // -- Implementation --
    template <typename... Args>
    Connection<Args...>::Connection(const Signal<Args...>& signal, std::unique_ptr<InternalConnectionMethod<Args...>> internalConnection)
        : signal(signal)
        , internalConnectionMethod(std::move(internalConnection))
    {
    }

    template <typename... Args>
    Connection<Args...>::Connection(const Signal<Args...>& signal, std::unique_ptr<InternalConnectionFunction<Args...>> internalConnection)
        : signal(signal)
        , internalConnectionFunction(std::move(internalConnection))
    {
    }

    template <typename... Args>
    Connection<Args...>::Connection(const Signal<Args...>& signal, std::unique_ptr<InternalConnectionLambda<Args...>> internalConnection)
        : signal(signal)
        , internalConnectionLambda(std::move(internalConnection))
    {
    }

    template <typename... Args>
    void Connection<Args...>::operator()(Args... args) const
    {
        if (internalConnectionMethod)
        {
            (*internalConnectionMethod)(std::forward<Args>(args)...);
        }
        else if (internalConnectionFunction)
        {
            (*internalConnectionFunction)(std::forward<Args>(args)...);
        }
        else if (internalConnectionLambda)
        {
            (*internalConnectionLambda)(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Connection<Args...>::disconnect()
    {
        signal.disconnect(this);
    }
}
