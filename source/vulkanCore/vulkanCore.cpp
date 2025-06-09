#include "vulkanCore.hpp"
#include "VN_logger.hpp"
#include "instance.hpp"
#include "logicalDevice.hpp"
#include "physicalDevice.hpp"
#include "window.hpp"

#include <cassert>

namespace venus {

	VulkanCore::VulkanCore(std::unique_ptr<Window> const &windowRef) {
		m_instance = std::make_unique<Instance>(windowRef->getAppDetails());
		windowRef->createSurface();
		storeSurface(windowRef->getSurfaceHandle());
		m_physicalDevice = std::make_unique<PhysicalDevice>(m_surfaceHandleRef);
		m_logicalDevice = std::make_unique<LogicalDevice>(m_surfaceHandleRef, m_physicalDevice);
		VN_LOG_INFO("Vulkan Context construction was successful.");
	}

	VulkanCore::~VulkanCore() {
		m_logicalDevice.reset();
		m_physicalDevice.reset();
		m_instance.reset();
		VN_LOG_INFO("Vulkan Context destruction was successful.");
	}

	[[nodiscard]] auto VulkanCore::getInstanceHandle() const -> VkInstance { return m_instance->getHandle(); }

	void VulkanCore::storeSurface(VkSurfaceKHR windowSurface) {
		m_surfaceHandleRef = windowSurface;
		if(m_surfaceHandleRef != nullptr) {
			VN_LOG_INFO("Surface handshake completed successfully.");
		}
	}

}  // namespace venus
