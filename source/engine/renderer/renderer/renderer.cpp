#include "renderer.hpp"
#include "VN_logger.hpp"
#include "graphicsPipeline.hpp"
#include "logicalDevice.hpp"
#include "renderConfig.hpp"
#include "swapchain.hpp"
#include "window.hpp"

// STDLIB
#include <chrono>

namespace venus {
	namespace {  // ANONYMOUS NAMESPACE BEGIN
		void logLoopTime() {
			static constexpr uint32_t TIME_LIMIT = 5;
			static auto LAST_MESSAGE_TIME = std::chrono::steady_clock::now();

			static uint32_t loopCount = 0;
			++loopCount;

			auto NOW = std::chrono::steady_clock::now();
			auto ELAPSED = std::chrono::duration_cast<std::chrono::seconds>(NOW - LAST_MESSAGE_TIME);

			if(ELAPSED.count() >= TIME_LIMIT) {
				VN_LOG_TRACE(std::format("[Frame Count {}] Time Elapsed: {}", loopCount, ELAPSED.count()));
				LAST_MESSAGE_TIME = NOW;
			}
		}
	}  // namespace
	// ANONYMOUS NAMEPSACE END

	Renderer::Renderer(const std::shared_ptr<Window> &windowPtr): m_window(windowPtr) {
		m_logicalDevice = std::make_shared<LogicalDevice>(m_window->getSurfaceHandle());
		m_swapchain = std::make_shared<Swapchain>(m_window, m_logicalDevice);
		m_graphicsPipeline = std::make_unique<GraphicsPipeline>(m_logicalDevice, m_swapchain);

		createSyncObjects();
		VN_LOG_INFO("Venus Renderer has been created.");
	}

	Renderer::~Renderer() {
		destroySyncObjects();
		m_graphicsPipeline.reset();
		m_swapchain.reset();
		m_logicalDevice.reset();
		VN_LOG_INFO("Venus Renderer has been destroyed.");
	}

	void Renderer::draw() {
		vkWaitForFences(m_logicalDevice->getHandle(), 1, &inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_logicalDevice->getHandle(), 1, &inFlightFences[m_currentFrame]);

		uint32_t imageIndex = 0;
		vkAcquireNextImageKHR(m_logicalDevice->getHandle(), m_swapchain->getHandle(), UINT64_MAX,
													imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(m_logicalDevice->getCommandBuffers()[m_currentFrame], 0);
		recordDrawCommandBuffer(imageIndex);

		std::vector<VkSemaphore> waitSemaphores = {imageAvailableSemaphores[m_currentFrame]};
		std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

		std::vector<VkCommandBuffer> commandBuffers = {m_logicalDevice->getCommandBuffers()[m_currentFrame]};

		std::vector<VkSemaphore> signalSemaphores = {renderFinishedSemaphores[m_currentFrame]};

		VkSubmitInfo submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
														.pNext = nullptr,
														.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()),
														.pWaitSemaphores = waitSemaphores.data(),
														.pWaitDstStageMask = waitStages.data(),
														.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
														.pCommandBuffers = commandBuffers.data(),
														.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
														.pSignalSemaphores = signalSemaphores.data()};

		if(vkQueueSubmit(m_logicalDevice->getGraphicsQueue(), 1, &submitInfo, inFlightFences[m_currentFrame]) !=
			 VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to submit graphics queue.");
			throw std::runtime_error("Failed to submit graphics queue.");
		}

		std::vector<VkSwapchainKHR> swapchains = {m_swapchain->getHandle()};
		VkPresentInfoKHR presentInfo{.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
																 .pNext = nullptr,
																 .waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
																 .pWaitSemaphores = signalSemaphores.data(),
																 .swapchainCount = static_cast<uint32_t>(swapchains.size()),
																 .pSwapchains = swapchains.data(),
																 .pImageIndices = &imageIndex,
																 .pResults = nullptr};

		vkQueuePresentKHR(m_logicalDevice->getPresentQueue(), &presentInfo);

		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		logLoopTime();
	}

	void Renderer::createSyncObjects() {
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = nullptr, .flags = 0};
		VkFenceCreateInfo fenceInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
																.pNext = nullptr,
																.flags = VK_FENCE_CREATE_SIGNALED_BIT};

		for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			if(vkCreateSemaphore(m_logicalDevice->getHandle(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
					 VK_SUCCESS ||
				 vkCreateSemaphore(m_logicalDevice->getHandle(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
					 VK_SUCCESS ||
				 vkCreateFence(m_logicalDevice->getHandle(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				VN_LOG_CRITICAL("Failed to create synchronization objects.");
				throw std::runtime_error("Failed to create synchronization objects.");
			}
		}
	}

	void Renderer::destroySyncObjects() {
		for(auto &semaphore : imageAvailableSemaphores) {
			vkDestroySemaphore(m_logicalDevice->getHandle(), semaphore, nullptr);
		}

		for(auto &semaphore : renderFinishedSemaphores) {
			vkDestroySemaphore(m_logicalDevice->getHandle(), semaphore, nullptr);
		}

		for(auto &fence : inFlightFences) {
			vkDestroyFence(m_logicalDevice->getHandle(), fence, nullptr);
		}

		VN_LOG_INFO("Destroyed synchronization objects.");
	}

	void Renderer::recordDrawCommandBuffer(const uint32_t &imageIndex) {
		m_logicalDevice->start_RecordCommandBuffer(m_currentFrame);

		VkClearValue clearColor = {{{0.0F, 0.0F, 0.0F, 1.0F}}};
		VkRenderPassBeginInfo renderBeginInfo{.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
																					.pNext = nullptr,
																					.renderPass = m_swapchain->getRenderPass(),
																					.framebuffer = m_swapchain->getFrameBuffers()[imageIndex],
																					.renderArea = {{0, 0}, m_swapchain->getImageExtent()},
																					.clearValueCount = 1,
																					.pClearValues = &clearColor};
		vkCmdBeginRenderPass(m_logicalDevice->getCommandBuffers()[m_currentFrame], &renderBeginInfo,
												 VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_logicalDevice->getCommandBuffers()[m_currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS,
											m_graphicsPipeline->getHandle());

		VkViewport viewport{.x = 0.0F,
												.y = 0.0F,
												.width = static_cast<float>(m_swapchain->getImageExtent().width),
												.height = static_cast<float>(m_swapchain->getImageExtent().height),
												.minDepth = 0.0F,
												.maxDepth = 1.0F};
		VkRect2D scissor{
			.offset = {0, 0},
			.extent = m_swapchain->getImageExtent(),
		};

		vkCmdSetViewport(m_logicalDevice->getCommandBuffers()[m_currentFrame], 0, 1, &viewport);
		vkCmdSetScissor(m_logicalDevice->getCommandBuffers()[m_currentFrame], 0, 1, &scissor);

		vkCmdDraw(m_logicalDevice->getCommandBuffers()[m_currentFrame], 3, 1, 0, 0);
		vkCmdEndRenderPass(m_logicalDevice->getCommandBuffers()[m_currentFrame]);

		m_logicalDevice->stop_RecordCommandBuffer(m_currentFrame);
	}

}  // namespace venus
