#pragma once

#include <algorithm>
#include <list>
#include <memory>
#include <type_traits>

namespace coral
{
    // Declaration
    template <typename... Args>
    struct Signal;

    template <typename... Args>
    struct Connection;

    // Helpers
    /**
     * @brief Connect the signal to the object method
     * @return The connection object, can be used to disconnect from the signal
     * usage: connect<method>(signal, object);
     */
    template <auto Function, typename From, typename... Args>
    std::shared_ptr<Connection<Args...>> connect(const Signal<Args...>& signal, From* object);

    /**
     * @brief Disconnect the given connection
     * usage: disconnect(connection);
     */
    template <typename... Args>
    void disconnect(const Connection<Args...>& connection);

    /**
     * @brief The Connection handle a connection to a signal
     */
    template <typename... Args>
    struct Connection
    {
        /**
         * @brief Constructor with the signal and bind method
         */
        Connection(const Signal<Args...>& signal, void* function, void* object);

        /**
         * @brief Call the bind method
         */
        void operator()(Args... args) const;

        /**
         * @brief Disconnect the connection from the signal
         */
        void disconnect();

    private:
        // Data
        const Signal<Args...>& signal;
        mutable void* function;
        mutable void* object;
    };

    /**
     * @brief The Signal is a struct emitting a signal that can be bind to a function
     * usage: signal.connect<Class::method>(object);
     */
    template <typename... Args>
    struct Signal
    {
        // Count args to create specialized methods
        static constexpr bool numberOfArgs = sizeof...(Args) > 0;

        /**
         * @brief Connect the signal to an object method
         * @return The connection object, can be used to disconnect from the signal
         */
        template <typename From, void (From::*Function)(Args...)>
        std::shared_ptr<Connection<Args...>> connect(From* object) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this, reinterpret_cast<void*>(+[](void* object, Args... args) { ((*reinterpret_cast<From**>(object))->*Function)(args...); }), object);
            connections.push_back(connection);
            return connection;
        }

        /**
         * @brief Connect the signal to an object method
         * @return The connection object, can be used to disconnect from the signal
         */
        template <typename From, void (From::*Function)()>
        std::shared_ptr<Connection<Args...>> connect(typename std::enable_if<numberOfArgs, From*>::type object) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this, reinterpret_cast<void*>(+[](void* object, Args... args) { ((*reinterpret_cast<From**>(object))->*Function)(); }), object);
            connections.push_back(connection);
            return connection;
        }

        /**
         * @brief Disconnect the given connection
         */
        void disconnect(std::shared_ptr<Connection<Args...>> connection) const;

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
        mutable std::list<std::shared_ptr<Connection<Args...>>> connections;
    };

    // -- Implementation --
    template <typename... Args>
    Connection<Args...>::Connection(const Signal<Args...>& signal, void* function, void* object)
        : signal(signal)
        , function(function)
        , object(object)
    {
    }

    template <typename... Args>
    void Connection<Args...>::operator()(Args... args) const
    {
        reinterpret_cast<void (*)(void*, Args...)>(function)(&object, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Connection<Args...>::disconnect()
    {
        signal.disconnect(this);
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(std::shared_ptr<Connection<Args...>> connection) const
    {
        disconnect(connection.get());
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(Connection<Args...>* connection) const
    {
        auto it = std::find_if(connections.begin(), connections.end(), [connection](const std::shared_ptr<Connection<Args...>>& c) { return c.get() == connection; });
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
    std::shared_ptr<Connection<Args...>> connect(const Signal<Args...>& signal, From* object)
    {
        return signal.template connect<From, Function>(std::forward<From*>(object));
    }

    template <typename... Args>
    void disconnect(const Connection<Args...>& connection)
    {
        connection.disconnect();
    }
}
