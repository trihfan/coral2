#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
#include <memory>

namespace coral
{
    class Node;

    //
    class Scene : public Object
    {
    public:
        Scene();

        // Objects
        void add(std::shared_ptr<Node> node);
        void remove(std::shared_ptr<Node> node);

        // Get top node
        std::shared_ptr<Node> getTopNode() const;

    private:
        std::shared_ptr<Node> top_node;
    };
}
#endif
