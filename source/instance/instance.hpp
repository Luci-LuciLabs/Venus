#ifndef VENUS_INSTANCE_HPP
#define VENUS_INSTANCE_HPP

#include "appDetails.hpp"

// 3RD PARTY
#include "volk.h"

// STDLIB
#include <vector>

namespace venus {

	class Instance {
	public:
		Instance(AppDetails &detailsRef);
		~Instance();

		Instance(const Instance &) = delete;
		Instance &operator=(const Instance &) = delete;

		VkInstance getInstance(void) const { return m_instance; }

	private:
    AppDetails &m_details;
		VkInstance m_instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_layerMessenger = VK_NULL_HANDLE;
    
		std::vector<const char *> getRequiredExtensions(void);
		bool checkValidationLayerSupport(void);

		void setupLayerMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &messenger);
	};

}  // namespace venus

#endif  // VENUS_INSTANCE_HPP