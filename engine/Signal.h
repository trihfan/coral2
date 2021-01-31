#ifndef SIGNAL_H
#define SIGNAL_H

#include <memory>
#include <algorithm>
#include <list>

namespace coral
{
    struct Function
    {
        void* function;
        void* object = nullptr;
    };

    template<typename... Args>
	struct Signal;

    template<typename... Args>
    struct Connection
    {
        Connection(const Signal<Args...>& signal, Function&& function) : signal(signal), function(std::move(function)) {}
        Connection(Connection& other) = delete;

        void operator()(Args... args)
        {
            if (function.object == nullptr)
            {
                reinterpret_cast<void(*)(Args...)>(function.function)(std::forward<Args>(args)...);
            }
            else
            {
                reinterpret_cast<void(*)(void*, Args...)>(function.function)(&function.object, std::forward<Args>(args)...);
            }
        }

        void disconnect()
        {
            signal.disconnect(*this);
        }

    private:
        const Signal<Args...>& signal;
        Function function;
    };

    template<typename... Args>
	struct Signal
	{
        // connect
        std::shared_ptr<Connection<Args...>> connect(void(*func)(Args...)) const
        {
            Function function;
            function.function = reinterpret_cast<void*>(func);
            auto connection = std::make_shared<Connection<Args...>>(*this, std::move(function));
            connections.push_back(connection);
            return connection;
        }

        template<auto Method, class From>
		std::shared_ptr<Connection<Args...>> connect(From* object) const
		{
            Function function;
            function.object = object;
            function.function = reinterpret_cast<void*>(+[](void* obj, Args... args){((*reinterpret_cast<From**>(obj))->*Method)(args...); });
            auto connection = std::make_shared<Connection<Args...>>(*this, std::move(function));
            connections.push_back(connection);
            return connection;
        }

        // disconnect
        template<typename Type>
        void disconnect(Type connection) const
        {
            auto it = std::find(connections.begin(), connections.end(), connection);
            if (it != connections.end())
            {
                connections.erase(it);
            }
        }

        // emit signal
        void emit(Args... args)
        {
            for (std::shared_ptr<Connection<Args...>> connnection : connections)
            {
                (*connnection)(std::forward<Args>(args)...);
            }
        }

	private:
        mutable std::list<std::shared_ptr<Connection<Args...>>> connections;
	};

    template<typename... Args>
    void connect()
    {
        
    }
}
#endif
