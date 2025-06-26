#ifndef VENUS_INSTANCE_HPP
#define VENUS_INSTANCE_HPP

// PROJECT
#include "venusConfigOptions.hpp"

// THIRD PARTY
#include "volk.h"

namespace venus {
	/**
   * @brief Vulkan Instance object.
   *
   * @class Instance.
   *
   * @details This object has an explicit contstructor that depends on a fully populated ApplicationIdentityDetails structure.
   *          This object should never be moved nor copied. We use volk to acquire the instance from anywhere without passing the handle around. 
   *          This object manages the creation of a new VkInstance, as well acquisition of instance extensions and validation layers.
   *
   *          This object will manage the lifetime of 2 member handles, VkInstance and VkDebugUtilsMessengerEXT. 
   *          Upon destruction it will first destroy the debug-messenger, and then the vulkan instance.
   *          Since the vulkan instance is required as an interface into the vulkan loader it,
   *          should always be the final destruction of a vulkan component in any program.
   *
   *          All validation layers or extensions are handled in an anonymous namespace within the instance source file.
   *
   *          This object is considered necessary for the entirity of runtime, therefore we handle it with the boostrapper.
   */
	class Instance {
	public:
		explicit Instance(const ApplicationIdentityDetails &appID);
		~Instance();

		Instance(const Instance &) = delete;
		auto operator=(const Instance &) -> Instance & = delete;

		Instance(const Instance &&) = delete;
		auto operator=(const Instance &&) -> Instance & = delete;

		[[nodiscard]] auto getHandle() const -> VkInstance { return m_instance; }

	private:
		VkInstance m_instance = VK_NULL_HANDLE;

		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
		bool USING_VALIDATION_LAYERS = false;

		void setupLayerDebugger();
	};

}  // namespace venus

#endif  // VENUS_INSTANCE_HPP