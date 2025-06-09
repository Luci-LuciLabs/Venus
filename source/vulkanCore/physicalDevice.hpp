#ifndef VENUS_VULKAN_PHYSICAL_DEVICE_HPP
#define VENUS_VULKAN_PHYSICAL_DEVICE_HPP

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <optional>
#include <vector>

namespace venus {

	const std::vector<const char *> REQUIRED_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR supportedSurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
		std::vector<VkPresentModeKHR> supportedPresentModes;
	};

	// NOLINTBEGIN
	struct QueueFamilyIndices {
		// Every GPU conformant to vulkan must always support atleast 1 queue family containing
		// a graphics queue, a present queue, and a transfer queue.
		// from what I can tell the graphics queue and present queue are often the exact some queue
		// th
		std::optional<uint32_t> graphicsFamilyIndex;
		std::optional<uint32_t> presentFamilyIndex;
		std::optional<uint32_t> computeFamilyIndex;
		std::optional<uint32_t> transferFamilyIndex;
		bool supportsMinimum() {
			// from what I could find these are always the same queue and every graphics card conforming to vulkan
			// must support both, however its still encouraged to check both properly.
			return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
		}
		// this queue may or may not be in the same queue family as the graphics and present queues.
		bool supportsCompute() { return computeFamilyIndex.has_value(); }
		// this queue must always be supported on any graphics card, however it may not be a different queue family than
		// the graphics queue and present queue, i believe the requirement is all 3 must be supported AT MINIMUM.
		bool supportsTransfer() { return transferFamilyIndex.has_value(); }
	};
	// NOLINTEND

	class PhysicalDevice {
	public:
		explicit PhysicalDevice(const VkSurfaceKHR &surfaceRef);
		~PhysicalDevice();

		PhysicalDevice(const PhysicalDevice &) = delete;
		auto operator=(const PhysicalDevice &) -> PhysicalDevice & = delete;
		PhysicalDevice(const PhysicalDevice &&) = delete;
		auto operator=(const PhysicalDevice &&) -> PhysicalDevice & = delete;

		[[nodiscard]] auto getHandle() const -> VkPhysicalDevice { return m_gpuDevice; }
		[[nodiscard]] auto getQueueFamilyIndices() const -> QueueFamilyIndices { return m_gpuDevice_queueFamilyIndices; }
		[[nodiscard]] auto getSwapchainSupportDetails() const -> SwapchainSupportDetails {
			return m_gpuDevice_swapchainSupportDetails;
		}

	private:
		VkPhysicalDevice m_gpuDevice = VK_NULL_HANDLE;

		QueueFamilyIndices m_gpuDevice_queueFamilyIndices{};
		SwapchainSupportDetails m_gpuDevice_swapchainSupportDetails{};
	};

}  // namespace venus

#endif  // VENUS_VULKAN_PHYSICAL_DEVICE_HPP