#ifndef VENUS_PHYSICAL_DEVICE
#define VENUS_PHYSICAL_DEVICE

#include "volk.h"
#include "common.hpp"

// stdlib
#include <vector>

namespace venus::rendersystem {
	class physicalDevice {
	public:
		physicalDevice();
		~physicalDevice();

		/// @brief This class should never be copied. 
		physicalDevice(const physicalDevice &) = delete;
    /// @brief This class should never be copied. 
		physicalDevice &operator=(const physicalDevice &) = delete;
		/// @brief This class should never be moved. 
		physicalDevice(const physicalDevice &&) = delete;
    /// @brief This class should never be moved. 
		physicalDevice &operator=(const physicalDevice &&) = delete;

		/// @brief Picks best physical device available on host system. Requires a valid vulkan instance and valid window surface.
		/// @param instance 
		/// @param surface 
		/// @return bool
		bool pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

		VkPhysicalDevice getDevice(void) const { return m_physicalDevice; }
		VkPhysicalDeviceProperties getProperties(void) const { return m_deviceProperties; }
		VkPhysicalDeviceFeatures getFeatures(void) { return m_deviceFeatures; }
		SwapchainSupportDetails getSwapchainSupportDetails(void) const { return m_swapchainSupportDetails; }
		QueueFamilyIndices getQueueFamilyIndices(void) const { return m_queueFamilyIndices; }

	private:
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_deviceProperties;
		VkPhysicalDeviceFeatures m_deviceFeatures;
		SwapchainSupportDetails m_swapchainSupportDetails;
		QueueFamilyIndices m_queueFamilyIndices;

		// functions related to picking the best physical device
		///////////////////////////////////////////////////////////////////////
		bool deviceMeetsMinimumRequired(VkPhysicalDevice device, VkSurfaceKHR surface);
		bool areDeviceExtensionsSupported(VkPhysicalDevice device);
		uint32_t giveFoundDeviceScore(VkPhysicalDevice device);
		//////////////////////////////////////////////////////////////////////

		SwapchainSupportDetails querySwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
		QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
	};

}  // namespace venus::rendersystem

#endif  // VENUS_PHYSICAL_DEVICE