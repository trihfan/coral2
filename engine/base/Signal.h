#pragma once

#include "Connection.h"
#include "Ptr.h"
#include <algorithm>
#include <list>
#include <memory>
#include <type_traits>

namespace coral
{
    // Declaration
    template <typename... Args>
    class Signal;

    // Helpers
    /**
     * @brief Connect the signal to the object method
     * @return The connection object, can be used to disconnect from the signal
     * usage: connect<method>(signal, object);
     */
    template <auto Function, typename From, typename... Args>
    ptr<Connection<Args...>> connect(const Signal<Args...>& signal, From* object);

    /**
     * @brief Connect the signal to the function
     * @return The connection object, can be used to disconnect from the signal
     * usage: connect<method>(signal);
     */
    template <auto Function, typename... Args>
    ptr<Connection<Args...>> connect(const Signal<Args...>& signal);

    /**
     * @brief Connect the signal to the lambda
     * @return The connection object, can be used to disconnect from the signal
     * usage: connect(signal, lambda);
     */
    template <typename Function, typename... Args>
    ptr<Connection<Args...>> connect(const Signal<Args...>& signal, Function&& lambda);

    /**
     * @brief Disconnect the given connection
     * usage: disconnect(connection);
     */
    template <typename... Args>
    void disconnect(const Connection<Args...>& connection);

    /**
     * @brief The Signal is a struct emitting a signal that can be bind to a function
     * usage: signal.connect<Class::method>(object);
     */
    template <typename... Args>
    class Signal
    {
    public:
        // Count args to create specialized methods
        static constexpr bool numberOfArgs = sizeof...(Args) > 0;

        /**
         * @brief Connect the signal to an object method
         * @return The connection object, can be used to disconnect from the signal
         */
        template <typename From, void (From::*Function)(Args...)>
        ptr<Connection<Args...>> connect(From* object) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this,
                std::make_unique<InternalConnectionMethod<Args...>>(object, reinterpret_cast<void*>(+[](void* object, Args... args) { ((*reinterpret_cast<From**>(object))->*Function)(std::forward<Args>(args)...); })));
            connections.push_back(connection);
            return connection;
        }

        /**
         * @brief Connect the signal to an object method
         * @return The connection object, can be used to disconnect from the signal
         */
        template <typename From, void (From::*Function)()>
        ptr<Connection<Args...>> connect(typename std::enable_if<numberOfArgs, From*>::type object) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this,
                std::make_unique<InternalConnectionMethod<Args...>>(object, reinterpret_cast<void*>(+[](void* object, Args...) { ((*reinterpret_cast<From**>(object))->*Function)(); })));
            connections.push_back(connection);
            return connection;
        }

        /**
         * @brief Connect the signal to a function
         * @return The connection object, can be used to disconnect from the signal
         */
        template <void (*Function)(Args...)>
        ptr<Connection<Args...>> connect() const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this,
                std::make_unique<InternalConnectionFunction<Args...>>(reinterpret_cast<void*>(+[](Args... args) { (*Function)(std::forward<Args>(args)...); })));
            connections.push_back(connection);
            return connection;
        }

        /**
         * @brief Connect the signal to a lambda
         * @return The connection object, can be used to disconnect from the signal
         */
        ptr<Connection<Args...>> connect(std::function<void(Args...)>&& lambda) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this,
                std::make_unique<InternalConnectionLambda<Args...>>(std::forward<std::function<void(Args && ...)>>(lambda)));
            connections.push_back(connection);
            return connection;
        }

        /**
         * @brief Disconnect the given connection
         */
        void disconnect(ptr<Connection<Args...>> connection) const;

        /**
         * @brief Disconnect the given connection
         */
        void disconnect(Connection<Args...>* connection) const;

        /**
         * @brief Emit the signal
         */
        void operator()(Args... args) const;

    private:
        // List of connection connected to the signal
        mutable std::list<ptr<Connection<Args...>>> connections;
    };

    // -- Implementation --
    template <typename... Args>
    void Signal<Args...>::disconnect(ptr<Connection<Args...>> connection) const
    {
        disconnect(connection.get());
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(Connection<Args...>* connection) const
    {
        auto it = std::find_if(connections.begin(), connections.end(), [connection](const ptr<Connection<Args...>>& c) { return c.get() == connection; });
        if (it != connections.end())
        {
            connections.erase(it);
        }
    }

    template <typename... Args>
    void Signal<Args...>::operator()(Args... args) const
    {
        for (const auto& connnection : connections)
        {
            (*connnection)(std::forward<Args>(args)...);
        }
    }

    template <auto Function, typename From, typename... Args>
    ptr<Connection<Args...>> connect(const Signal<Args...>& signal, From* object)
    {
        return signal.template connect<From, Function>(std::forward<From*>(object));
    }

    template <auto Function, typename... Args>
    ptr<Connection<Args...>> connect(const Signal<Args...>& signal)
    {
        return signal.template connect<Function>();
    }

    template <typename Function, typename... Args>
    ptr<Connection<Args...>> connect(const Signal<Args...>& signal, Function&& lambda)
    {
        return signal.connect(std::forward<std::function<void(Args && ...)>>(lambda));
    }

    template <typename... Args>
    void disconnect(const Connection<Args...>& connection)
    {
        connection.disconnect();
    }
}
