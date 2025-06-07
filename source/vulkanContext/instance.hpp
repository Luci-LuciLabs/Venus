#ifndef VENUS_INSTANCE_HPP
#define VENUS_INSTANCE_HPP

#include "appDetails.hpp"
#include "volk.h"

namespace venus {
	/**
   * @brief Basic VkInstance handler class.
   */
	class Instance {
	public:
		/**
     * @brief Only class constructor available.
     * 
     * @param const AppDetails&
     */
		Instance(const AppDetails &detailsRef);
		~Instance();

		Instance(const Instance &) = delete;
		auto operator=(const Instance &) -> Instance & = delete;
		Instance(const Instance &&) = delete;
		auto operator=(const Instance &&) -> Instance & = delete;

		/**
     * @brief Get the Vulkan Instance object
     * 
     * @return  VkInstance [[nodiscard]]
     */
		[[nodiscard]] auto getInstance() const -> VkInstance { return m_instance; }

	private:
		/** @brief Handle for AppDetails, required in construction. */
		AppDetails m_details;
		/** @brief Handle for Vulkan Instance, required for everything after its creation. */
		VkInstance m_instance = VK_NULL_HANDLE;

		/** @brief Handle for Vulkan Instance specific validations layer messenger, remains unchanged if layers are not supported. */
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

		/** @brief Changes to true if validation layers are both requested and supported, used in destruction logic. */
		bool USING_VALIDATION_LAYERS = false;

		/** @brief Creates Vulkan Instance debugMessenger, sets "m_debugMessenger". */
		void setupLayerDebugger();
	};

}  // namespace venus

#endif  // VENUS_INSTANCE_HPP