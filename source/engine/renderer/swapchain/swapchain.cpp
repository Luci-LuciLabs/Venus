#include "swapchain.hpp"
#include "VN_logger.hpp"
#include "logicalDevice.hpp"
#include "renderConfig.hpp"
#include "window.hpp"

// STDLIB
#include <algorithm>
#include <cassert>
#include <limits>

namespace venus {

	namespace {  // ANONYMOUS NAMESPACE BEGIN

		auto chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &supportedFormats) -> VkSurfaceFormatKHR {
			assert(!supportedFormats.empty());

			// we attempt to find this format specifically since its the most ubiquitous and is our preference.
			// not finding it is incredibly unlikely so for now we simply return the first supported format.
			for(const auto &availableFormat : supportedFormats) {
				if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
					return availableFormat;
				}
			}

			return supportedFormats[0];
		}

		auto choosePresentMode(const std::vector<VkPresentModeKHR> &supportedPresentModes) -> VkPresentModeKHR {
			assert(supportedPresentModes.data() != nullptr);
			// It should be noted that among linux users with nvidia graphics cards triple buffering may not work at all,
			// unfortunately nvidia has poor support for linux users.
			// It should work perfectly fine for amd graphics cards however.
			for(const auto &presentMode : supportedPresentModes) {
				if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					VN_LOG_INFO("Using triple-buffering presentation.");
					return presentMode;
				}
			}

			// This is querying for adaptive vsync specifically to limit our rendering at this time.
			for(const auto &presentMode : supportedPresentModes) {
				if(presentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
					VN_LOG_INFO("Using adaptive v-sync presentation.");
					return presentMode;
				}
			}

			// default to v-sync since its always supported as is required in the vulkan spec.
			VN_LOG_INFO("Using v-sync presentation.");
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		auto chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, const VkExtent2D &currentWindowExtent) {
			if(surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return surfaceCapabilities.currentExtent;
			}

			VkExtent2D clamped_extent = {
				.width = std::clamp(currentWindowExtent.width, surfaceCapabilities.minImageExtent.width,
														surfaceCapabilities.maxImageExtent.width),
				.height = std::clamp(currentWindowExtent.height, surfaceCapabilities.minImageExtent.height,
														 surfaceCapabilities.maxImageExtent.height)};

			return clamped_extent;
		}

	}  // namespace
	// ANONYMOUS NAMESPACE END

	Swapchain::Swapchain(const std::shared_ptr<Window> &windowPtr,
											 const std::shared_ptr<LogicalDevice> &logicalDevicePtr):
		m_window(windowPtr), m_logicalDevice(logicalDevicePtr) {
		auto swapchainSupport = m_logicalDevice->swapchainSupportDetails();
		auto chosenPresentMode = choosePresentMode(swapchainSupport.supportedPresentModes);
		auto chosenFormat = chooseSurfaceFormat(swapchainSupport.supportedSurfaceFormats);
		auto chosenExtent =
			chooseSwapExtent(swapchainSupport.supportedSurfaceCapabilities, m_window->getCurrentSurfaceExtent());

		uint32_t imageCount = swapchainSupport.supportedSurfaceCapabilities.minImageCount + 1;
		if(swapchainSupport.supportedSurfaceCapabilities.maxImageCount > 0 &&
			 imageCount > swapchainSupport.supportedSurfaceCapabilities.maxImageCount) {
			imageCount = swapchainSupport.supportedSurfaceCapabilities.maxImageCount;
		}
		imageCount = std::min(imageCount, MAX_FRAMES_IN_FLIGHT);

		static constexpr uint8_t IMAGE_LAYER_COUNT = 1;

		auto indices = m_logicalDevice->queueFamilyIndices();
		std::vector<uint32_t> queueindices = {indices.graphicsFamilyIndex.value(),  // NOLINT
																					indices.presentFamilyIndex.value()};  // NOLINT

		const VkSwapchainCreateInfoKHR createInfo = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.surface = m_window->getSurfaceHandle(),
			.minImageCount = imageCount,
			.imageFormat = chosenFormat.format,
			.imageColorSpace = chosenFormat.colorSpace,
			.imageExtent = chosenExtent,
			.imageArrayLayers = IMAGE_LAYER_COUNT,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.preTransform = swapchainSupport.supportedSurfaceCapabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = chosenPresentMode,
			.clipped = VK_TRUE,
			.oldSwapchain = VK_NULL_HANDLE};

		if(vkCreateSwapchainKHR(m_logicalDevice->getHandle(), &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create swapchain, swapchain is nullptr.");
			throw std::runtime_error("Failed to create swapchain, swapchain is nullptr.");
		}

		vkGetSwapchainImagesKHR(m_logicalDevice->getHandle(), m_swapchain, &imageCount, nullptr);
		m_swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice->getHandle(), m_swapchain, &imageCount, m_swapchainImages.data());

		m_imageExtent = chosenExtent;
		m_imageFormat = chosenFormat.format;

		createImageViews();
		createRenderPass();
		createFrameBuffers();
		VN_LOG_INFO("Swapchain construction was successful.");
	}

	Swapchain::~Swapchain() {
		for(auto &framebuffer : m_frameBuffers) {
			vkDestroyFramebuffer(m_logicalDevice->getHandle(), framebuffer, nullptr);
		}

		vkDestroyRenderPass(m_logicalDevice->getHandle(), m_renderPass, nullptr);

		for(auto &imageView : m_swapchainImageViews) {
			vkDestroyImageView(m_logicalDevice->getHandle(), imageView, nullptr);
		}

		assert(m_swapchain != nullptr);
		vkDestroySwapchainKHR(m_logicalDevice->getHandle(), m_swapchain, nullptr);
		VN_LOG_INFO("Swapchain destruction was successful.");
	}

	void Swapchain::createImageViews() {
		assert(!m_swapchainImages.empty());
		m_swapchainImageViews.resize(m_swapchainImages.size());

		for(size_t i = 0; i < m_swapchainImages.size(); ++i) {
			const VkComponentMapping componentMap{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
																						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
																						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
																						.a = VK_COMPONENT_SWIZZLE_IDENTITY};

			const VkImageSubresourceRange subResourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
																										 .baseMipLevel = 0,
																										 .levelCount = 1,
																										 .baseArrayLayer = 0,
																										 .layerCount = 1};

			const VkImageViewCreateInfo viewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
																					 .pNext = nullptr,
																					 .flags = 0,
																					 .image = m_swapchainImages[i],
																					 .viewType = VK_IMAGE_VIEW_TYPE_2D,
																					 .format = m_imageFormat,
																					 .components = componentMap,
																					 .subresourceRange = subResourceRange};

			if(vkCreateImageView(m_logicalDevice->getHandle(), &viewInfo, nullptr, &m_swapchainImageViews[i]) != VK_SUCCESS) {
				VN_LOG_INFO("Failed to create swapchain image views.");
				throw std::runtime_error("Failed to create swapchain image views.");
			}
		}
	}

	void Swapchain::createFrameBuffers() {
		m_frameBuffers.resize(m_swapchainImageViews.size());
		for(size_t i = 0; i < m_swapchainImageViews.size(); ++i) {
			std::vector<VkImageView> attachments = {m_swapchainImageViews[i]};

			const VkFramebufferCreateInfo frameBufferInfo{.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
																										.pNext = nullptr,
																										.flags = 0,
																										.renderPass = m_renderPass,
																										.attachmentCount = 1,
																										.pAttachments = attachments.data(),
																										.width = m_imageExtent.width,
																										.height = m_imageExtent.height,
																										.layers = 1};

			if(vkCreateFramebuffer(m_logicalDevice->getHandle(), &frameBufferInfo, nullptr, &m_frameBuffers[i]) !=
				 VK_SUCCESS) {
				VN_LOG_CRITICAL("Failed to create framebuffer.");
				throw std::runtime_error("Failed to create framebuffer.");
			}
		}
	}

	void Swapchain::createRenderPass() {
		const VkAttachmentDescription colorAttachmentDescription{.flags = 0,
																														 .format = m_imageFormat,
																														 .samples = VK_SAMPLE_COUNT_1_BIT,
																														 .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
																														 .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
																														 .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
																														 .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
																														 .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
																														 .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};

		const VkAttachmentReference colorAttachmentReference{.attachment = 0,
																												 .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

		const VkSubpassDescription subpassDescription{.flags = 0,
																									.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
																									.inputAttachmentCount = 0,
																									.pInputAttachments = nullptr,
																									.colorAttachmentCount = 1,
																									.pColorAttachments = &colorAttachmentReference,
																									.pResolveAttachments = nullptr,
																									.pDepthStencilAttachment = nullptr,
																									.preserveAttachmentCount = 0,
																									.pPreserveAttachments = nullptr};

		const VkSubpassDependency subpassDependency{.srcSubpass = VK_SUBPASS_EXTERNAL,
																								.dstSubpass = 0,
																								.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
																								.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
																								.srcAccessMask = 0,
																								.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
																								.dependencyFlags = 0};

		const VkRenderPassCreateInfo renderPassInfo{.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
																								.pNext = nullptr,
																								.flags = 0,
																								.attachmentCount = 1,
																								.pAttachments = &colorAttachmentDescription,
																								.subpassCount = 1,
																								.pSubpasses = &subpassDescription,
																								.dependencyCount = 1,
																								.pDependencies = &subpassDependency};

		if(vkCreateRenderPass(m_logicalDevice->getHandle(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create renderpass.");
			throw std::runtime_error("Failed to create renderpass.");
		}
	}

}  // namespace venus