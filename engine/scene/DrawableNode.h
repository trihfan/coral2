#ifndef DRAWABLENODE_H
#define DRAWABLENODE_H

#include <vector>
#include <memory>
#include "Node.h"

namespace coral
{
	struct RenderParameters;
	class Material;

	// A node represent an drawable item in the scene
	class DrawableNode : public Node
	{
	public:
		// construction
		DrawableNode(std::shared_ptr<Node> parent = nullptr);
	
		// meta
		bool isDrawable() const override;

		// render queue
		void setRenderQueue(unsigned int render_queue);
		unsigned int getRenderQueue() const;

		// material
        void setMaterial(std::shared_ptr<Material> material);
        std::shared_ptr<Material> getMaterial() const;

		// draw
		virtual void draw(const RenderParameters& parameters) = 0;

	protected:
		std::shared_ptr<Material> material;
		unsigned int render_queue = 1000;
	};
}
#endif
