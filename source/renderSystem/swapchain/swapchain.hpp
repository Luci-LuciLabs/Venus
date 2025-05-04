#ifndef VENUS_SWAPCHAIN_HPP
#define VENUS_SWAPCHAIN_HPP

#include "volk.h"
#include "common.hpp"

// STDLIB
#include <vector>

namespace venus::rendersystem {
	class swapchain {
	public:
		swapchain();
		~swapchain();

		swapchain(const swapchain &) = delete;
		swapchain &operator=(const swapchain &) = delete;

		bool createSwapchain(VkDevice logicalDevice, VkSurfaceKHR surface, VkExtent2D windowExtent,
												 SwapchainSupportDetails details, QueueFamilyIndices indices);
		void destroySwapchain(VkDevice logicalDevice);

		void createImageViews(void);

		VkSwapchainKHR getSwapchain(void) const { return m_swapchain; }

	private:
		VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
		VkFormat m_imageFormat;
		VkExtent2D m_imageExtent;
		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imageViews;

		VkSurfaceFormatKHR chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
		VkPresentModeKHR chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
		VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, VkExtent2D windowExtent);
	};

}  // namespace venus::rendersystem

#endif  // VENUS_SWAPCHAIN_HPP