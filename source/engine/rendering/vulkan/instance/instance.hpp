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
		[[nodiscard]] auto getHandle() const -> VkInstance { return m_instance; }

	private:
		AppDetails m_details;
		VkInstance m_instance = VK_NULL_HANDLE;

		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
		bool USING_VALIDATION_LAYERS = false;

		void setupLayerDebugger();
	};

}  // namespace venus

#endif  // VENUS_INSTANCE_HPP