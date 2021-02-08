#ifndef SIGNAL_H
#define SIGNAL_H

#include <memory>
#include <algorithm>
#include <list>
#include <type_traits>

namespace coral
{
    // Declaration
    template<typename... Args>
	struct Signal;

    template<typename... Args>
    struct Connection;

    template<auto Function, typename From, typename... Args>
    std::shared_ptr<Connection<Args...>> connect(const Signal<Args...>& signal, From* object);

    template<typename... Args>
    void disconnect(const Connection<Args...>& connection);

    // Connection, handle calling the function
    template<typename... Args>
    struct Connection
    {
        // Construction
        Connection(const Signal<Args...>& signal, void* function, void* object);

        // Call the encapsulated function
        void operator()(Args... args) const;

        // Disconnect the connection
        void disconnect();

    private:
        // Data
        const Signal<Args...>& signal;
        mutable void* function;
        mutable void* object;
    };

    template<typename... Args>
	struct Signal
	{
        static constexpr bool numberOfArgs = sizeof...(Args) > 0;

        // connect
        template<typename From, void(From::*Function)(Args...)>
		std::shared_ptr<Connection<Args...>> connect(From* object) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this, reinterpret_cast<void*>(+[](void* object, Args... args) {((*reinterpret_cast<From**>(object))->*Function)(args...); }), object);
            connections.push_back(connection);
            return connection;
        }

        template<typename From, void(From::*Function)()>
		std::shared_ptr<Connection<Args...>> connect(typename std::enable_if<numberOfArgs, From*>::type object) const
        {
            auto connection = std::make_shared<Connection<Args...>>(*this, reinterpret_cast<void*>(+[](void* object, Args... args) {((*reinterpret_cast<From**>(object))->*Function)(); }), object);
            connections.push_back(connection);
            return connection;
        }

        // disconnect
        void disconnect(std::shared_ptr<Connection<Args...>> connection) const;
        void disconnect(Connection<Args...>* connection) const;

        // emit signal
        void operator()(Args... args) const;

	private:
        mutable std::list<std::shared_ptr<Connection<Args...>>> connections;
	};

    template<typename... Args>
    Connection<Args...>::Connection(const Signal<Args...>& signal, void* function, void* object) : signal(signal), function(function), object(object) {}

    template<typename... Args>
    void Connection<Args...>::operator()(Args... args) const
    {
        reinterpret_cast<void(*)(void*, Args...)>(function)(&object, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Connection<Args...>::disconnect()
    {
        signal.disconnect(this);
    }

    template<typename... Args>
    void Signal<Args...>::disconnect(std::shared_ptr<Connection<Args...>> connection) const
    {
        disconnect(connection.get());
    }

    template<typename... Args>
    void Signal<Args...>::disconnect(Connection<Args...>* connection) const
    {
        auto it = std::find_if(connections.begin(), connections.end(), [connection](const std::shared_ptr<Connection<Args...>>& c){ return c.get() == connection; });
        if (it != connections.end())
        {
            connections.erase(it);
        }
    }

    template<typename... Args>
    void Signal<Args...>::operator()(Args... args) const
    {
        for (const auto& connnection : connections)
        {
            (*connnection)(std::forward<Args>(args)...);
        }
    }

    template<auto Function, typename From, typename... Args>
    std::shared_ptr<Connection<Args...>> connect(const Signal<Args...>& signal, From* object)
    {
        return signal.template connect<From, Function>(std::forward<From*>(object));
    }

    template<typename... Args>
    void disconnect(const Connection<Args...>& connection)
    {
        connection.disconnect();
    }
}
#endif
