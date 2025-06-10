#ifndef VENUS_VULKAN_CORE_HPP
#define VENUS_VULKAN_CORE_HPP
// PROJECT

// THIRD PARTY
#include "volk.h"

// STDLIB
#include <memory>

namespace venus {
	class Window;
	class Instance;
	class PhysicalDevice;
	class LogicalDevice;

	class VulkanCore {
	public:
		explicit VulkanCore(std::unique_ptr<Window> const &windowRef);
		~VulkanCore();

		VulkanCore(const VulkanCore &) = delete;
		auto operator=(const VulkanCore &) -> VulkanCore & = delete;
		VulkanCore(const VulkanCore &&) = delete;
		auto operator=(const VulkanCore &&) -> VulkanCore & = delete;

		[[nodiscard]] auto getInstanceHandle() const -> VkInstance;

	private:
		VkSurfaceKHR m_surfaceHandleRef = VK_NULL_HANDLE;
		std::unique_ptr<Instance> m_instance;
		std::unique_ptr<PhysicalDevice> m_physicalDevice;
		std::unique_ptr<LogicalDevice> m_logicalDevice;

		void storeSurface(VkSurfaceKHR windowSurface);
	};

}  // namespace venus

#endif  // VENUS_VULKAN_CORE_HPP