#include "instance.hpp"
#include "VN_logger.hpp"
#include "appDetails.hpp"
#include "version.hpp"

#include <cassert>
#include <cstring>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>
// this define prevents glfw from including any opengl specific content.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace venus {
	namespace {  // ANONYMOUS NAMESPACE BEGIN
#ifdef NDEBUG
		constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
		constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

		// To update validation layers you wish to use, you may adjust this vector below.
		const std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation",
																												 "VK_LAYER_LUNARG_crash_diagnostic", "VK_LAYER_LUNARG_monitor",
																												 "VK_LAYER_KHRONOS_synchronization2"};
		// To properly use your desired validation layers you must also add any required extensions to this vector.
		const std::vector<const char *> VALIDATION_EXTENSIONS = {"VK_EXT_debug_utils"};

		// DO NOT CHANGE. This vector will be loaded into the instanceExtensions vector used in VkInstanceCreateInfo{}.
		const std::vector<const char *> REQUIRED_EXTENSIONS = {"VK_KHR_get_physical_device_properties2"};

		auto getRequiredExtensions() -> std::vector<const char *> {
			uint32_t extensionCount = 0;
			// glfwGetRequiredInstanceExtensions will automatically load any window/surface extensions necessary for glfw-target platform.
			// keep in mind that this may vary across systems, due to GLFW compilation requirements. EX: wayland vs x11.
			// this function can be called from any thread, and possibly before glfwInit() however you should err on the side of caution.
			const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

			const std::span<const char *> extensionSpan(glfwExtensions, extensionCount);
			std::vector<const char *> extensions(extensionSpan.begin(), extensionSpan.end());

			for(const char *extension : REQUIRED_EXTENSIONS) {
				extensions.push_back(extension);
			}

			if(ENABLE_VALIDATION_LAYERS) {
				for(const char *extension : VALIDATION_EXTENSIONS) {
					extensions.push_back(extension);
				}
			}

			return extensions;
		}

		auto checkValidationLayerSupport() -> bool {
			const bool SUPPORT_CHECK_SUCCESS = true;
			const bool SUPPORT_CHECK_FAILURE = false;

			auto func = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(
				vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceLayerProperties"));

			if(func != nullptr) {
				uint32_t layerCount = 0;
				func(&layerCount, nullptr);

				std::vector<VkLayerProperties> availableLayers(layerCount);
				func(&layerCount, availableLayers.data());

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

			VN_LOG_WARN(
				"(vkGetInstanceProcAddr) failed to acquire (PFN_vkEnumerateInstanceLayerProperties), function pointer is "
				"null.");
			return SUPPORT_CHECK_FAILURE;
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
        VN_LOG_ERROR(callback_msg);
      }else{
        VN_LOG_DEBUG(callback_msg);
      }   

    return VK_FALSE;
    }
		// clang-format on
	}  // namespace

	Instance::Instance(const AppDetails &detailsRef): m_details(detailsRef) {
		// glfw is initialized within the window class, which requires that this Instance class be initialized after Window.
		if(volkInitialize() == VK_SUCCESS) {
			VN_LOG_INFO("Volk-Loader initialized successfully.");
		} else {
			const std::string volk_fail_msg = "Volk-Loader failed to initialize";
			VN_LOG_CRITICAL(volk_fail_msg);
			throw std::runtime_error(volk_fail_msg);
		}

		static const bool requiredVulkanVersionFound = [] {
			VN_LOG_INFO("Checking Vulkan version...");
			auto func =
				reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
			if(func != nullptr) {
				uint32_t versionFound = 0;
				func(&versionFound);
				VN_LOG_INFO("Found Vulkan 1.4+ on system.");
				return versionFound >= VK_API_VERSION_1_4;
			}
			VN_LOG_INFO("Did not find Vulkan 1.4+ on system.");
			return false;
		}();

		if(!requiredVulkanVersionFound) {
			const std::string vulkan_version_msg = "Failed to find required version of vulkan.";
			VN_LOG_CRITICAL(vulkan_version_msg);
			throw std::runtime_error(vulkan_version_msg);
		}

		static const auto &instanceLayers = [] {
			return (ENABLE_VALIDATION_LAYERS && checkValidationLayerSupport()) ? VALIDATION_LAYERS :
																																					 std::vector<const char *>{};
		}();

		// our use of VkApplicationInfo{} is dependant upon version.hpp and appDetails.hpp
		const VkApplicationInfo appInfo{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = m_details.applicationName,
			.applicationVersion = VK_MAKE_VERSION(m_details.versionMajor, m_details.versionMinor, m_details.versionPatch),
			.pEngineName = "Venus",
			.engineVersion = VK_MAKE_VERSION(VENUS_MAJOUR, VENUS_MINOUR, VENUS_PATCH),
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

		auto func = reinterpret_cast<PFN_vkCreateInstance>(vkGetInstanceProcAddr(nullptr, "vkCreateInstance"));
		if(func != nullptr) {
			if(func(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
				const std::string create_error_message = "Failed to create VkInstance, instance is nullptr.";
				VN_LOG_CRITICAL(create_error_message);
				throw std::runtime_error(create_error_message);
			}
			volkLoadInstance(m_instance);
		} else {
			const std::string func_error_message =
				"(vkGetInstanceProcAddr) failed to acquire (PFN_vkCreateInstance), function pointer is null.";
			VN_LOG_WARN(func_error_message);
			throw std::runtime_error(func_error_message);
		}

		if(!instanceLayers.empty()) {
			USING_VALIDATION_LAYERS = true;
			setupLayerDebugger();
		}

		VN_LOG_INFO("Class::Instance, Construction finished.");
	}

	Instance::~Instance() {
		assert(m_instance != VK_NULL_HANDLE);

		if(USING_VALIDATION_LAYERS) {
			auto debug_func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
				vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
			if(debug_func != nullptr) {
				debug_func(m_instance, m_debugMessenger, nullptr);
				VN_LOG_INFO("Destroyed validation-layer debug-messenger.");
			} else {
				VN_LOG_WARN(
					"(vkGetInstanceProcAddr) failed to acquire (PFN_vkDestroyDebugUtilsMessengerEXT), function pointer is null.");
				m_debugMessenger = VK_NULL_HANDLE;
			}
		}

		auto inst_func = reinterpret_cast<PFN_vkDestroyInstance>(vkGetInstanceProcAddr(m_instance, "vkDestroyInstance"));
		if(inst_func != nullptr) {
			inst_func(m_instance, nullptr);
			m_instance = VK_NULL_HANDLE;
			VN_LOG_INFO("Destroyed vulkan instance.");
		} else {
			VN_LOG_WARN("(vkGetInstanceProcAddr) failed to acquire (PFN_vkDestroyInstance), function pointer is null.");
			m_instance = VK_NULL_HANDLE;
		}
		VN_LOG_INFO("Class::Instance, Destruction finished.");
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

		auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
		if(func != nullptr) {
			func(m_instance, &createInfo, nullptr, &m_debugMessenger);
			return;
		}
		VN_LOG_WARN(
			"(vkGetInstanceProcAddr) failed to acquire (PFN_vkCreateDebugUtilsMessengerEXT), function pointer is null.");
	}

}  // namespace venus