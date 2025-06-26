#ifndef VENUS_PHYSICAL_DEVICE_HPP
#define VENUS_PHYSICAL_DEVICE_HPP

// PROJECT
#include "gpuStructures.hpp"

// THIRD PARTY
#include "volk.h"

namespace venus {

	const std::vector<const char *> REQUIRED_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
																												 VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME};

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

#endif  // VENUS_PHYSICAL_DEVICE_HPP