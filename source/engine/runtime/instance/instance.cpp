#include "instance.hpp"
#include "VN_logger.hpp"
#include "version.hpp"

#include <cassert>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>
// this define prevents glfw from including any opengl specific content.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace venus {
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	namespace {  // ANONYMOUS NAMESPACE BEGIN
#if defined(NDEBUG)
		constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
		constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

		// To update validation layers you wish to use, you may adjust this vector below.
		const std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

		// To properly use your desired validation layers you must also add any required extensions to this vector.
		const std::vector<const char *> VALIDATION_EXTENSIONS = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		// DO NOT CHANGE. This vector will be loaded into the instanceExtensions vector used in VkInstanceCreateInfo{}.
		const std::vector<const char *> MANDATORY_EXTENSIONS = {VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
																														VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME};

		auto getRequiredExtensions() -> std::vector<const char *> {
			// glfwGetRequiredInstanceExtensions will automatically load any window/surface extensions necessary for glfw-target platform.
			// keep in mind that this may vary across systems, due to GLFW compilation requirements. EX: wayland vs x11.
			// this function can be called from any thread, and possibly before glfwInit() however you should err on the side of caution.

			uint32_t glfwExtensionCount = 0;
			const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			if(glfwExtensions == nullptr) {
				throw std::runtime_error("Failed to acquire required glfw instance extensions.");
			}

			const std::span<const char *> extensionSpan(glfwExtensions, glfwExtensionCount);
			std::vector<const char *> REQUIRED_EXTENSIONS(extensionSpan.begin(), extensionSpan.end());
			for(const char *extension : MANDATORY_EXTENSIONS) {
				REQUIRED_EXTENSIONS.push_back(extension);
			}

			if(ENABLE_VALIDATION_LAYERS) {
				for(const char *extension : VALIDATION_EXTENSIONS) {
					REQUIRED_EXTENSIONS.push_back(extension);
				}
			}

			// We must validate that the required extensions are available to avoid undefined behaviour.
			uint32_t availableExtensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

			for(const char *extensionRequired : REQUIRED_EXTENSIONS) {
				bool extensionFound = false;

				for(auto extensionAvailable : availableExtensions) {
					const std::string_view extensionName = static_cast<const char *>(extensionAvailable.extensionName);
					if(extensionRequired == extensionName) {
						extensionFound = true;
						break;
					}
				}
				if(!extensionFound) {
					VN_LOG_CRITICAL("Required instance extensions are not available.");
					throw std::runtime_error("Required instnace extensions are not available.");
				}
			}

			return REQUIRED_EXTENSIONS;
		}

		auto checkValidationLayerSupport() -> bool {
			constexpr bool SUPPORT_CHECK_SUCCESS = true;
			constexpr bool SUPPORT_CHECK_FAILURE = false;

			uint32_t layerCount = 0;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for(const char *layerName : VALIDATION_LAYERS) {
				bool layerFound = false;

				for(const auto &layerProperties : availableLayers) {
					const std::basic_string_view availableName = static_cast<const char *>(layerProperties.layerName);

					if(layerName == availableName) {
						layerFound = true;
						break;
					}
				}

				if(!layerFound) {
					VN_LOG_WARN("Validation layers requested but not supported.");
					return SUPPORT_CHECK_FAILURE;
				}
			}
			VN_LOG_INFO("All requested validation layers have been found.");
			return SUPPORT_CHECK_SUCCESS;
		}

		// clang-format off
		VKAPI_ATTR auto VKAPI_CALL debuggerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																								VkDebugUtilsMessageTypeFlagsEXT messageType,
																								const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
																								void *pUserData) -> VkBool32 
    {
      // supressing required params that go unused.
      // required by vulkan.
      (void)messageType;
      (void)pUserData;

      const std::string_view callback_msg = pCallbackData->pMessage;
      if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
        if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT){
          VN_LOG_CRITICAL(callback_msg);
        }else{
          VN_LOG_ERROR(callback_msg);
        }
      }else{
        if(messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT){
          VN_LOG_DEBUG(callback_msg);
        }else{
          VN_LOG_INFO(callback_msg);
        }
      }   

    return VK_FALSE;
    }


	}  // ANONYMOUS NAMESPACE END
	// clang-format on
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Instance::Instance(const ApplicationIdentityDetails &appID) {
		static const auto &instanceLayers = [] {
			return (ENABLE_VALIDATION_LAYERS && checkValidationLayerSupport()) ? VALIDATION_LAYERS :
																																					 std::vector<const char *>{};
		}();

		// our use of VkApplicationInfo{} is dependant upon version.hpp and appDetails.hpp
		const VkApplicationInfo appInfo{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = appID.name,
			.applicationVersion = VK_MAKE_VERSION(appID.version.major, appID.version.minor, appID.version.patch),
			.pEngineName = "Venus",
			.engineVersion = VK_MAKE_VERSION(VENUS_MAJOR, VENUS_MINOR, VENUS_PATCH),
			.apiVersion = VK_API_VERSION_1_4};

		std::vector<const char *> instanceExtensions = getRequiredExtensions();

		const VkInstanceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = VK_NULL_HANDLE,
			.flags = 0,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size()),
			.ppEnabledLayerNames = instanceLayers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
			.ppEnabledExtensionNames = instanceExtensions.data(),
		};

		if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
			VN_LOG_CRITICAL("Failed to create VkInstance, instance is nullptr.");
			throw std::runtime_error("Failed to create VkInstance, instance is nullptr.");
		}

		volkLoadInstance(m_instance);

		if(!instanceLayers.empty()) {
			USING_VALIDATION_LAYERS = true;
			setupLayerDebugger();
		}
		VN_LOG_INFO("Venus Instance has been created.");
	}

	Instance::~Instance() {
		assert(m_instance != VK_NULL_HANDLE);

		if(USING_VALIDATION_LAYERS) {
			vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
			m_debugMessenger = VK_NULL_HANDLE;
			VN_LOG_INFO("Venus Instance validation-layer debug-messenger has been destroyed.");
		}

		vkDestroyInstance(m_instance, nullptr);
		m_instance = VK_NULL_HANDLE;
		VN_LOG_INFO("Venus Instance has been destroyed.");
	}

	void Instance::setupLayerDebugger() {
		// DO NOT ENABLE "VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_BINDING_ADDRESS_BIT", it requires device extensions and
		// should only ever be implemented into the device debug messenger.
		const VkDebugUtilsMessengerCreateInfoEXT createInfo = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = VK_NULL_HANDLE,
			.flags = 0,
			.messageSeverity =
				(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
				 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT),

			.messageType = (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
											VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT),

			.pfnUserCallback = debuggerCallback,
			.pUserData = VK_NULL_HANDLE};

		if(vkCreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
			VN_LOG_ERROR("Failed to create Venus Instance validation-layer debug-messenger.");
		}
		VN_LOG_INFO("Venus Instance validation-layer debug-messenger has been created.");
	}

}  // namespace venus