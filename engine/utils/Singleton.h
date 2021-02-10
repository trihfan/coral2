#include <memory>
#include "utils/Logs.h"

// Make a singleton class 
#define MAKE_SINGLETON(X) \
private: \
    void release(); \
    X(const X&) = delete; \
    X& operator=(const X&) = delete; \
    X(X&&) = delete; \
    X& operator=(X&&) = delete; \
    static std::unique_ptr<X> instance; \
    template <class... Args> static void createInstance(Args&&... args) \
    { \
        if (instance) { Logs(warning) << #X << " already created, recreating"; } \
        instance = std::unique_ptr<X>(new X(std::forward<Args>(args)...)); \
        Logs(success) << #X << " created"; \
    } \
    static void destroyInstance() { instance->release(); instance = nullptr; Logs(success) << #X << " destroyed"; }

// Make a class a singleton of the engine, the method to release the instance must be provided
#define MAKE_ENGINE_SINGLETON(X) \
private: \
    friend class coral::Engine; \
    MAKE_SINGLETON(X)

#define DEFINE_SINGLETON(X) std::unique_ptr<X> X::instance; void X::release() {}
