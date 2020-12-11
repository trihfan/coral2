#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <string>
#include <vector>

namespace coral
{
    // The Object class represent an object in coral engine
    class Object : public std::enable_shared_from_this<Object>
    {
    public:
        // construction
        Object();
        virtual ~Object();
        void destroy();

        // initialization
        virtual void init() {};
        virtual void release() {};

        // name
        void setName(const std::string& name);
        const std::string& getName() const;

    private:
        bool destroyed;
        std::string name;
    };

    class ObjectInstance
    {
    public:
        static void registerObject(std::shared_ptr<Object> object);
        static void unregisterObject(std::shared_ptr<Object> object);

    private:
        static std::vector<std::shared_ptr<Object>> objects;
        static std::vector<std::shared_ptr<Object>> initialize_list;
        static std::vector<std::shared_ptr<Object>> release_list;
    };
}
#endif