#include "logicalDevice.hpp"
#include "VN_logger.hpp"
#include "physicalDevice.hpp"

// STDLIB
#include <cassert>
#include <set>

namespace venus {

	LogicalDevice::LogicalDevice(const VkSurfaceKHR &surfaceRef,
															 std::unique_ptr<PhysicalDevice> const &physicalDeviceRef): m_surface(surfaceRef) {
		assert(m_surface != nullptr);

		const auto indices = physicalDeviceRef->getQueueFamilyIndices();
		const std::set<std::optional<uint32_t>> uniqueQueueFamilies = {indices.graphicsFamilyIndex,
																																	 indices.presentFamilyIndex};

		const float QUEUE_PRIORITY = 1.0;
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for(std::optional<uint32_t> family : uniqueQueueFamilies) {
			if(!(family == std::nullopt)) {
				const VkDeviceQueueCreateInfo queueInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
																								.pNext = nullptr,
																								.flags = 0,
																								.queueFamilyIndex = family.value(),
																								.queueCount = 1,
																								.pQueuePriorities = &QUEUE_PRIORITY};
				queueCreateInfos.push_back(queueInfo);
			}
		}

		// TODO: MOVE TO PHYSICAL DEVICE CLASS
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(physicalDeviceRef->getHandle(), &features);

		const VkDeviceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
			.pQueueCreateInfos = queueCreateInfos.data(),
			.enabledLayerCount = 0,          // DEPRECATED DO NOT USE
			.ppEnabledLayerNames = nullptr,  // DEPRECATED DO NOT USE
			.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_EXTENSIONS.size()),
			.ppEnabledExtensionNames = REQUIRED_EXTENSIONS.data(),
			.pEnabledFeatures = &features  // TODO: Move this to physical device class.
		};

		if(vkCreateDevice(physicalDeviceRef->getHandle(), &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create Logical Device, handle is nullptr.");
			throw std::runtime_error("Failed to create Logical Device, handle is nullpt.");
		}

		VN_LOG_INFO("Logical Device construction was successful.");
	}

	LogicalDevice::~LogicalDevice() {
		assert(m_logicalDevice != VK_NULL_HANDLE);

		vkDestroyDevice(m_logicalDevice, nullptr);
		m_logicalDevice = VK_NULL_HANDLE;

		VN_LOG_INFO("Logical Device destruction was successful.");
	}

}  // namespace venus