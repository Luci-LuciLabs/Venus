#ifndef VENUS_RENDERER_HPP
#define VENUS_RENDERER_HPP

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <memory>
#include <vector>

namespace venus {
	class Window;
	class LogicalDevice;
	class Swapchain;
	class GraphicsPipeline;
	class Renderer {
	public:
		explicit Renderer(const std::shared_ptr<Window> &windowPtr);
		~Renderer();

		Renderer(const Renderer &) = delete;
		auto operator=(const Renderer &) -> Renderer & = delete;

		Renderer(const Renderer &&) = delete;
		auto operator=(const Renderer &&) -> Renderer & = delete;

		void draw();

	private:
		std::shared_ptr<Window> m_window;
		std::shared_ptr<LogicalDevice> m_logicalDevice;
		std::shared_ptr<Swapchain> m_swapchain;
		std::unique_ptr<GraphicsPipeline> m_graphicsPipeline;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		void createSyncObjects();
		void destroySyncObjects();

		void recordDrawCommandBuffer(const uint32_t &imageIndex);
		uint32_t m_currentFrame = 0;
	};

}  // namespace venus

#endif  // VENUS_RENDERER_HPP