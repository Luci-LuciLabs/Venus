#ifndef VENUS_SWAPCHAIN_HPP
#define VENUS_SWAPCHAIN_HPP

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <memory>
#include <vector>

namespace venus {
	class Window;
	class LogicalDevice;
	class Swapchain {
	public:
		explicit Swapchain(const std::shared_ptr<Window> &windowPtr,
											 const std::shared_ptr<LogicalDevice> &logicalDevicePtr);
		~Swapchain();

		explicit Swapchain(const Swapchain &) = delete;
		auto operator=(const Swapchain &) -> Swapchain & = delete;

		Swapchain(const Swapchain &&) = delete;
		auto operator=(const Swapchain &&) -> Swapchain & = delete;

		[[nodiscard]] auto getHandle() const { return m_swapchain; }

		[[nodiscard]] auto getImageExtent() const { return m_imageExtent; }
		[[nodiscard]] auto getImageFormat() const { return m_imageFormat; }
		[[nodiscard]] auto getImages() const { return m_swapchainImages; }
		[[nodiscard]] auto getImageViews() const { return m_swapchainImageViews; }

		[[nodiscard]] auto getRenderPass() const { return m_renderPass; }
		[[nodiscard]] auto getFrameBuffers() const { return m_frameBuffers; }

	private:
		VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

		VkRenderPass m_renderPass = VK_NULL_HANDLE;
		void createRenderPass();

		std::vector<VkImage> m_swapchainImages;
		VkExtent2D m_imageExtent{};
		VkFormat m_imageFormat{};
		std::vector<VkImageView> m_swapchainImageViews;
		void createImageViews();

		std::vector<VkFramebuffer> m_frameBuffers;
		void createFrameBuffers();

		std::shared_ptr<Window> m_window;
		std::shared_ptr<LogicalDevice> m_logicalDevice;
	};

}  // namespace venus

#endif  // VENUS_SWAPCHAIN_HPP