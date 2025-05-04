#ifndef VENUS_LOGICAL_DEVICE_HPP
#define VENUS_LOGICAL_DEVICE_HPP

#include "volk.h"
#include "common.hpp"

// STDLIB

namespace venus::rendersystem {
	class logicalDevice {
	public:
		logicalDevice();
		~logicalDevice();

		// THIS CLASS SHOULD NEVER BE COPIED.
		logicalDevice(const logicalDevice &) = delete;
		logicalDevice &operator=(const logicalDevice &) = delete;

		bool createLogicalDevice(VkPhysicalDevice physicalDevice,
														 VkPhysicalDeviceFeatures deviceFeatures, QueueFamilyIndices indices);
		void destroyLogicalDevice(void);

		VkDevice getLogicalDevice(void) const { return m_logicalDevice; }
		VkQueue getGraphicsQueue(void) const { return m_graphicsQueue; }
		VkQueue getPresentQueue(void) const { return m_presentQueue; }

	private:
		VkDevice m_logicalDevice = VK_NULL_HANDLE;

		VkQueue m_graphicsQueue = VK_NULL_HANDLE;
		VkQueue m_presentQueue = VK_NULL_HANDLE;
	};

}  // namespace venus::rendersystem

#endif  // VENUS_LOGICAL_DEVICE_HPP