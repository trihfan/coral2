#include "Logs.h"
#include <memory>

// Make a singleton class
#define MAKE_SINGLETON(X)                                                        \
private:                                                                         \
    void release();                                                              \
    X(const X&) = delete;                                                        \
    X& operator=(const X&) = delete;                                             \
    X(X&&) = delete;                                                             \
    X& operator=(X&&) = delete;                                                  \
    static std::shared_ptr<X> instance;                                          \
                                                                                 \
public:                                                                          \
    template <class... Args>                                                     \
    static void create(Args&&... args)                                           \
    {                                                                            \
        if (instance)                                                            \
        {                                                                        \
            coral::Logs(coral::warning) << #X << " already created, recreating"; \
        }                                                                        \
        instance = std::shared_ptr<X>(new X(std::forward<Args>(args)...));       \
        coral::Logs(coral::success) << #X << " created";                         \
    }                                                                            \
    static void destroy()                                                        \
    {                                                                            \
        instance->release();                                                     \
        instance = nullptr;                                                      \
        coral::Logs(coral::success) << #X << " destroyed";                       \
    }                                                                            \
    static std::shared_ptr<X> get()                                              \
    {                                                                            \
        return instance;                                                         \
    }

#define DEFINE_SINGLETON(X) std::shared_ptr<X> X::instance;
