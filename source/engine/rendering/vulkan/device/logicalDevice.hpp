#ifndef VENUS_VULKAN_LOGICAL_DEVICE_HPP
#define VENUS_VULKAN_LOGICAL_DEVICE_HPP

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <memory>

namespace venus {
	class PhysicalDevice;
	class LogicalDevice {
	public:
		explicit LogicalDevice(const VkSurfaceKHR &surfaceRef, std::unique_ptr<PhysicalDevice> const &physicalDeviceRef);
		~LogicalDevice();

		LogicalDevice(const LogicalDevice &) = delete;
		auto operator=(const LogicalDevice &) -> LogicalDevice & = delete;

		LogicalDevice(const LogicalDevice &&) = delete;
		auto operator=(const LogicalDevice &&) -> LogicalDevice & = delete;

		[[nodiscard]] auto getHandle() const -> VkDevice { return m_logicalDevice; }

	private:
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkDevice m_logicalDevice = VK_NULL_HANDLE;
	};

}  // namespace venus

#endif  // VENUS_VULKAN_LOGICAL_DEVICE_HPP