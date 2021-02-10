#ifndef DRAWABLENODE_H
#define DRAWABLENODE_H

#include <vector>
#include <memory>
#include "Node.h"
#include "renderpasses/RenderPassManager.h"

namespace coral
{
	struct RenderParameters;
	class Material;

	// A node represent an drawable item in the scene
	class DrawableNode : public Node
	{
	public:
		// construction
		DrawableNode(std::shared_ptr<Node> parent = nullptr, const std::vector<std::string>& renderQueueTags = { coral::defaultRenderPassName });
	
		// meta
		bool isDrawable() const override;

		// render queue
		void addRenderQueueTag(const std::string& renderQueueId);
		void removeRenderQueueTag(const std::string& renderQueueId);
		bool isTagForRenderQueue(const std::string& renderQueueId) const;
		const std::vector<std::string>& getRenderQueueTags() const;

		// material
        void setMaterial(std::shared_ptr<Material> material);
        std::shared_ptr<Material> getMaterial() const;

		// draw
		virtual void draw(const RenderParameters& parameters) = 0;

	protected:
		std::shared_ptr<Material> material;
		std::vector<std::string> renderQueueTags;
	};
}
#endif
