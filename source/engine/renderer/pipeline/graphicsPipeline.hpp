#ifndef VENUS_GRAPHICS_PIPELINE_HPP
#define VENUS_GRAPHICS_PIPELINE_HPP

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <memory>

namespace venus {
	class LogicalDevice;
	class Swapchain;
	class GraphicsPipeline {
	public:
		explicit GraphicsPipeline(const std::shared_ptr<LogicalDevice> &logicalDevicePtr,
															const std::shared_ptr<Swapchain> &swapchainPtr);
		~GraphicsPipeline();

		GraphicsPipeline(const GraphicsPipeline &) = delete;
		auto operator=(const GraphicsPipeline &) -> GraphicsPipeline & = delete;

		GraphicsPipeline(const GraphicsPipeline &&) = delete;
		auto operator=(const GraphicsPipeline &&) -> GraphicsPipeline & = delete;

		[[nodiscard]] auto getHandle() const { return m_graphicsPipeline; }

	private:
		VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

		std::shared_ptr<LogicalDevice> m_logicalDevice;
		std::shared_ptr<Swapchain> m_swapchain;
	};

}  // namespace venus

#endif  // VENUS_GRAPHICS_PIPELINE_HPP