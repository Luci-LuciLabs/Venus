#include "swapchain.hpp"

// STDLIB
#include <limits>

namespace venus::rendersystem {

	swapchain::swapchain(){};
	swapchain::~swapchain(){};

	bool swapchain::createSwapchain(VkDevice logicalDevice, VkSurfaceKHR surface, VkExtent2D windowExtent,
																	SwapchainSupportDetails details, QueueFamilyIndices indices) 
  {

		VkSurfaceFormatKHR chosenFormat = chooseSwapchainSurfaceFormat(details.surfaceFormats);
		VkPresentModeKHR chosenMode = chooseSwapchainPresentMode(details.presentModes);
		VkExtent2D chosenExtent = chooseSwapchainExtent(details.surfaceCapabilities, windowExtent);

    uint32_t imageCount = details.surfaceCapabilities.minImageCount + 1;

    if(details.surfaceCapabilities.maxImageCount > 0 && imageCount > details.surfaceCapabilities.minImageCount){
      imageCount = details.surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = chosenFormat.format;
    createInfo.imageColorSpace = chosenFormat.colorSpace;
    createInfo.imageExtent = chosenExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    if(indices.graphicsFamily != indices.presentFamily){
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }else{
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = details.surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = chosenMode;
    createInfo.clipped = VK_TRUE;

    if(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS){
      return false;
    }

    return true;
	}

	void swapchain::destroySwapchain(VkDevice logicalDevice) {
    if(m_swapchain != VK_NULL_HANDLE){
      vkDestroySwapchainKHR(logicalDevice, m_swapchain, nullptr);
    }
    m_swapchain = VK_NULL_HANDLE;
  }

	VkSurfaceFormatKHR swapchain::chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
		for(const auto &format : availableFormats) {
			if(format.format == VK_FORMAT_B8G8R8A8_SRGB) {
				return format;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR swapchain::chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
		for(const auto &presentMode : availablePresentModes) {
			if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return presentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D swapchain::chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, VkExtent2D windowExtent) {
    if(surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
      return surfaceCapabilities.currentExtent;
    }

    VkExtent2D actualExtent = windowExtent;
    actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }

}  // namespace venus::rendersystem
