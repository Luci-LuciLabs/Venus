#include "logicalDevice.hpp"
#include "VN_logger.hpp"
#include "physicalDevice.hpp"
#include "renderConfig.hpp"

// STDLIB
#include <cassert>
#include <set>
#include <stdexcept>

namespace venus {

	LogicalDevice::LogicalDevice(const VkSurfaceKHR &surfaceRef): m_surface(surfaceRef) {
		assert(m_surface != nullptr);

		m_physicalDevice = std::make_unique<PhysicalDevice>(surfaceRef);

		const auto indices = m_physicalDevice->getQueueFamilyIndices();
		const std::set<std::optional<uint32_t>> uniqueQueueFamilies = {indices.graphicsFamilyIndex,
																																	 indices.presentFamilyIndex};

		const float QUEUE_PRIORITY = 1.0;
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for(std::optional<uint32_t> family : uniqueQueueFamilies) {
			if(!(family == std::nullopt)) {
				const VkDeviceQueueCreateInfo queueInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
																								.pNext = nullptr,
																								.flags = 0,
																								.queueFamilyIndex = family.value(),
																								.queueCount = 1,
																								.pQueuePriorities = &QUEUE_PRIORITY};
				queueCreateInfos.push_back(queueInfo);
			}
		}

		// TODO: MOVE TO PHYSICAL DEVICE CLASS
		VkPhysicalDeviceSynchronization2Features syncFeatures2 = {};
		syncFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
		syncFeatures2.synchronization2 = VK_TRUE;

		VkPhysicalDeviceFeatures2 features2 = {};
		features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		features2.pNext = &syncFeatures2;

		vkGetPhysicalDeviceFeatures2(m_physicalDevice->getHandle(), &features2);

		const VkDeviceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &features2,
			.flags = 0,
			.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
			.pQueueCreateInfos = queueCreateInfos.data(),
			.enabledLayerCount = 0,          // DEPRECATED DO NOT USE
			.ppEnabledLayerNames = nullptr,  // DEPRECATED DO NOT USE
			.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_EXTENSIONS.size()),
			.ppEnabledExtensionNames = REQUIRED_EXTENSIONS.data(),
			.pEnabledFeatures = nullptr  // TODO: Move this to physical device class.
		};

		if(vkCreateDevice(m_physicalDevice->getHandle(), &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create Logical Device, handle is nullptr.");
			throw std::runtime_error("Failed to create Logical Device, handle is nullpt.");
		}

		volkLoadDevice(m_logicalDevice);

		if(indices.graphicsFamilyIndex.has_value()) {
			vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamilyIndex.value(), 0, &m_graphicsQueue);
		}

		if(indices.presentFamilyIndex.has_value()) {
			vkGetDeviceQueue(m_logicalDevice, indices.presentFamilyIndex.value(), 0, &m_presentQueue);
		}

		if(m_graphicsQueue == VK_NULL_HANDLE || m_presentQueue == VK_NULL_HANDLE) {
			VN_LOG_CRITICAL("Failed to successfully get device queues.");
			throw std::runtime_error("Failed to successfully get device queues.");
		}

		createCommandPool();
		createCommandBuffer();

		VN_LOG_INFO("Logical Device construction was successful.");
	}

	LogicalDevice::~LogicalDevice() {
		assert(m_logicalDevice != VK_NULL_HANDLE);

		vkDestroyCommandPool(m_logicalDevice, m_graphicsPool, nullptr);

		vkDestroyDevice(m_logicalDevice, nullptr);
		m_logicalDevice = VK_NULL_HANDLE;

		VN_LOG_INFO("Logical Device destruction was successful.");
	}

	auto LogicalDevice::queueFamilyIndices() const -> QueueFamilyIndices {
		return m_physicalDevice->getQueueFamilyIndices();
	}
	auto LogicalDevice::swapchainSupportDetails() const -> SwapchainSupportDetails {
		return m_physicalDevice->getSwapchainSupportDetails();
	}

	void LogicalDevice::createCommandPool() {
		auto indices = m_physicalDevice->getQueueFamilyIndices();
		uint32_t INDEX = 0;
		if(indices.graphicsFamilyIndex.has_value()) {
			INDEX = indices.graphicsFamilyIndex.value();
		}

		const VkCommandPoolCreateInfo createInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
																						 .pNext = nullptr,
																						 .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
																						 .queueFamilyIndex = INDEX};

		if(vkCreateCommandPool(m_logicalDevice, &createInfo, nullptr, &m_graphicsPool) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create graphics pool.");
			throw std::runtime_error("Failed to create graphics pool.");
		}
	}

	void LogicalDevice::createCommandBuffer() {
		m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		const VkCommandBufferAllocateInfo allocInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
																								.pNext = nullptr,
																								.commandPool = m_graphicsPool,
																								.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
																								.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size())};

		if(vkAllocateCommandBuffers(m_logicalDevice, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to allocate command buffer.");
			throw std::runtime_error("Failed to allocate command buffer.");
		}
	}

	void LogicalDevice::start_RecordCommandBuffer(const uint32_t &bufferIndex) {
		const VkCommandBufferBeginInfo beginInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
																						 .pNext = nullptr,
																						 .flags = 0,
																						 .pInheritanceInfo = nullptr};

		if(vkBeginCommandBuffer(m_commandBuffers[bufferIndex], &beginInfo) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to begin recording graphics buffer.");
			throw std::runtime_error("Failed to begin recording graphics buffer.");
		}
	}

	void LogicalDevice::stop_RecordCommandBuffer(const uint32_t &bufferIndex) {
		if(vkEndCommandBuffer(m_commandBuffers[bufferIndex]) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to record graphics buffer.");
			throw std::runtime_error("Failed to record graphics buffer.");
		}
	}

}  // namespace venus