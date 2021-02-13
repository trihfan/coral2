#include "utils/Logs.h"
#include <memory>

// Make a singleton class
#define MAKE_SINGLETON(X)                                                  \
private:                                                                   \
    void release();                                                        \
    X(const X&) = delete;                                                  \
    X& operator=(const X&) = delete;                                       \
    X(X&&) = delete;                                                       \
    X& operator=(X&&) = delete;                                            \
    static std::unique_ptr<X> instance;                                    \
                                                                           \
public:                                                                    \
    template <class... Args>                                               \
    static void create(Args&&... args)                                     \
    {                                                                      \
        if (instance)                                                      \
        {                                                                  \
            Logs(warning) << #X << " already created, recreating";         \
        }                                                                  \
        instance = std::unique_ptr<X>(new X(std::forward<Args>(args)...)); \
        Logs(success) << #X << " created";                                 \
    }                                                                      \
    static void destroy()                                                  \
    {                                                                      \
        instance->release();                                               \
        instance = nullptr;                                                \
        Logs(success) << #X << " destroyed";                               \
    }

#define DEFINE_SINGLETON(X) std::unique_ptr<X> X::instance;
