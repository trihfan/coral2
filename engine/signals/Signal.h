#pragma once
#include "Connection.h"
#include <algorithm>
#include <vector>
#include <memory>
#include <type_traits>
#include <functional>
#include <mutex>

namespace coral
{
    // The Signal is a struct emitting a signal that can be bind to a function
    template <typename... Args>
    class Signal
    {
    public:
        // Connect the signal to an object method
        template <typename... Args2>
        std::shared_ptr<Connection<Args...>> connect(Args2&&...args) const
        {
            std::lock_guard<std::mutex> lock(connectionsMutex);
            auto connection = std::make_shared<Connection<Args...>>(*this, std::forward<Args2>(args)...);
            connections.push_back(connection);
            return connection;
        }

        // Disconnect the given connection
        void disconnect(std::shared_ptr<Connection<Args...>> connection) const { disconnect(connection.get()); }
        void disconnect(Connection<Args...>* connection) const
        {
            std::lock_guard<std::mutex> lock(connectionsMutex);
            auto it = std::find_if(connections.begin(), connections.end(), [connection](const std::shared_ptr<Connection<Args...>>& c) { return c.get() == connection; });
            if (it != connections.end())
            {
                connections.erase(it);
            }
        }

        // Emit the signal
        void operator()(Args... args) const
        {
            connectionsMutex.lock();
            auto copy = connections;
            connectionsMutex.unlock();

            for (const auto& connnection : copy)
            {
                (*connnection)(std::forward<Args>(args)...);
            }
        }

    private:
        // List of connection connected to the signal
        mutable std::mutex connectionsMutex;
        mutable std::vector<std::shared_ptr<Connection<Args...>>> connections;
    };

    // -- Helpers --
    template <typename Signal, typename... Args>
    auto connect(const Signal& signal, Args&&... args)
    {
        return signal.template connect<Args...>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void disconnect(const Connection<Args...>& connection)
    {
        connection.disconnect();
    }

    template <typename... Args>
    void disconnect(const std::shared_ptr<Connection<Args...>>& connection)
    {
        connection->disconnect();
    }
}
