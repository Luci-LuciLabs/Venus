#ifndef VENUS_LOGICAL_DEVICE_HPP
#define VENUS_LOGICAL_DEVICE_HPP

// PROJECT
#include "gpuStructures.hpp"

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <memory>

namespace venus {
	class PhysicalDevice;
	class LogicalDevice {
	public:
		explicit LogicalDevice(const VkSurfaceKHR &surfaceRef);
		~LogicalDevice();

		LogicalDevice(const LogicalDevice &) = delete;
		auto operator=(const LogicalDevice &) -> LogicalDevice & = delete;

		LogicalDevice(const LogicalDevice &&) = delete;
		auto operator=(const LogicalDevice &&) -> LogicalDevice & = delete;

		[[nodiscard]] auto getHandle() const -> VkDevice { return m_logicalDevice; }

		[[nodiscard]] auto queueFamilyIndices() const -> QueueFamilyIndices;
		[[nodiscard]] auto swapchainSupportDetails() const -> SwapchainSupportDetails;

		[[nodiscard]] auto getCommandBuffers() const { return m_commandBuffers; }
		[[nodiscard]] auto getGraphicsQueue() const { return m_graphicsQueue; }
		[[nodiscard]] auto getPresentQueue() const { return m_presentQueue; }

		void start_RecordCommandBuffer(const uint32_t &bufferIndex);
		void stop_RecordCommandBuffer(const uint32_t &bufferIndex);

	private:
		std::unique_ptr<PhysicalDevice> m_physicalDevice;
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkDevice m_logicalDevice = VK_NULL_HANDLE;

		VkQueue m_graphicsQueue = VK_NULL_HANDLE;
		VkQueue m_presentQueue = VK_NULL_HANDLE;

		VkCommandPool m_graphicsPool = VK_NULL_HANDLE;
		void createCommandPool();

		std::vector<VkCommandBuffer> m_commandBuffers;
		void createCommandBuffer();
	};

}  // namespace venus

#endif  // VENUS_LOGICAL_DEVICE_HPP