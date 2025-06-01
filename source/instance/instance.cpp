#include "instance.hpp"
#include "version.hpp"
#include "VN_logger.hpp"

// 3RD PARTY
#include <GLFW/glfw3.h>

// STDLIB
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>

namespace venus {

  	// clang-format off
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																										 VkDebugUtilsMessageTypeFlagsEXT messageType,
																										 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
																										 void *pUserData) 
  {
      // these params are unused and cause warnings/errors during debug builds.
      // they are also required by vulkan for this callback to work as far as I am aware.
      // that is why we cast them as void.
      (void)messageSeverity;
      (void)messageType;
      (void)pUserData;
      std::string callbackMessage = pCallbackData->pMessage;
      std::string LAYER_MESSAGE = "Validations-Layer: " + callbackMessage + "\n";
      
      if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT){
        VN_LOG_DEBUG(LAYER_MESSAGE);
      }else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
        VN_LOG_WARN(LAYER_MESSAGE);
      }else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT){
        VN_LOG_ERROR(LAYER_MESSAGE);
      }else{
        VN_LOG_DEBUG(LAYER_MESSAGE);
      }
      
      //std::cerr << "Validation Layer: " << pCallbackData->pMessage << '\n';
      return VK_FALSE;                                                  
  }
    
	VkResult createDebugUtilsMessengerEXT(VkInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
																				const VkAllocationCallbacks *pAllocator,
																				VkDebugUtilsMessengerEXT *pDebugMessenger)
  {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
      if(func != nullptr){
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
      }else{
        VN_LOG_ERROR("Could not acquire vkCreateDebugUtilsMessengerEXT proc addr.");
        return VK_ERROR_EXTENSION_NOT_PRESENT;
      }
  }

	void destroyDebugUtilsMessengerEXT(VkInstance &instance, VkDebugUtilsMessengerEXT debugMessenger,
																		 const VkAllocationCallbacks *pAllocator)
  {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func != nullptr){
      func(instance, debugMessenger, pAllocator);
    }
    VN_LOG_INFO("Validations-Layer debugger has been destroyed.");
  }

	// clang-format on

	static void glfwErrorCallbackFunc(int error, const char *desc) {
    (void)error;
    std::string err_desc = desc;
    std::string message = "GLFW-ERROR: " + err_desc + "\n";
		VN_LOG_ERROR(message);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Instance::Instance(AppDetails &detailsRef) : m_details(detailsRef) {
    glfwSetErrorCallback(glfwErrorCallbackFunc);
		if(glfwInit() != GLFW_TRUE) {
      VN_LOG_CRITICAL("Failed to intialize GLFW.");
			throw std::runtime_error("Failed to initialize glfw.");
		}

		if(volkInitialize() != VK_SUCCESS) {
      VN_LOG_CRITICAL("Failed to initialize volk.");
			throw std::runtime_error("Failed to intialize volk.");
		}

    bool LAYERS_UNAVAILABLE = false;
		if(ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
      VN_LOG_WARN("Validation layers requested but not available, will continue without validation layers.");
      LAYERS_UNAVAILABLE = true;
		}

    uint32_t VULKAN_VERSION_FOUND;
    if(vkEnumerateInstanceVersion(&VULKAN_VERSION_FOUND) != VK_SUCCESS){
      VN_LOG_CRITICAL("Failed to find Vulkan.");
      throw std::runtime_error("Failed to find Vulkan.");
    }

    uint32_t maj = VK_API_VERSION_MAJOR(VULKAN_VERSION_FOUND);
    uint32_t min = VK_API_VERSION_MINOR(VULKAN_VERSION_FOUND);
    uint32_t pat = VK_API_VERSION_PATCH(VULKAN_VERSION_FOUND);
    std::string versionMessage = "Found Vulkan Version: " + std::to_string(maj) + "." + std::to_string(min) + "." + std::to_string(pat)  + '\n';
    VN_LOG_INFO(versionMessage);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    // ENGINE DETAILS
    appInfo.pEngineName = "Vsadf";
    appInfo.engineVersion = VK_MAKE_VERSION(VENUS_MAJOUR, VENUS_MINOUR, VENUS_PATCH);
    
    if(VULKAN_VERSION_FOUND < VK_MAKE_VERSION(1, 4, 0)){
      std::string warnVersion = "Vulkan Version 1.4 not found, using Version: " + std::to_string(maj) + "." + std::to_string(min) + '\n';
      VN_LOG_WARN(warnVersion);
      appInfo.apiVersion = VULKAN_VERSION_FOUND;
    }else{
      VN_LOG_INFO("Using preferred Vulkan Version: 1.4");
      appInfo.apiVersion = VK_API_VERSION_1_4; // DO NOT CHANGE. Venus is intended to use vulkan_1.4.xxx
    }

    // APP DETAILS
    appInfo.pApplicationName = m_details.name;
    appInfo.applicationVersion = VK_MAKE_VERSION(m_details.versionMajour, m_details.versionMinour, m_details.versionPatch);

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(LAYERS_UNAVAILABLE == true || ENABLE_VALIDATION_LAYERS == false){
      createInfo.enabledLayerCount = 0;
      createInfo.ppEnabledLayerNames = nullptr;
    }else{
      createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
      createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }

    if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS){
      throw std::runtime_error("Failed to create vulkan instance.");
    }

    volkLoadInstance(m_instance);
    setupLayerMessenger(m_instance, m_layerMessenger);
	}





	Instance::~Instance() {

    if(ENABLE_VALIDATION_LAYERS){
      destroyDebugUtilsMessengerEXT(m_instance, m_layerMessenger, nullptr);
    }
    m_layerMessenger = VK_NULL_HANDLE;

    if(m_instance != VK_NULL_HANDLE){
      vkDestroyInstance(m_instance, nullptr);
    }
    m_instance = VK_NULL_HANDLE;


    glfwTerminate();
    volkFinalize();
  }

	std::vector<const char *> Instance::getRequiredExtensions(void) {
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if(ENABLE_VALIDATION_LAYERS) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool Instance::checkValidationLayerSupport(void) {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for(const char *layerName : VALIDATION_LAYERS) {
			bool layerFound = false;

			for(const auto &layerProperties : availableLayers) {
				if(strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if(!layerFound) {
				return false;
			}
		}

		return true;
	}

	void Instance::setupLayerMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger) {
		if(!ENABLE_VALIDATION_LAYERS) {
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity =
			(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

		debugCreateInfo.messageType =
			(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

		debugCreateInfo.pfnUserCallback = debugCallback;
		debugCreateInfo.pUserData = nullptr;

		if(createDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
      VN_LOG_CRITICAL("Failed to create Validations-Layer-Debugger.");
			throw std::runtime_error("Failed to create Validations-Layer-Debugger.");
		}

    VN_LOG_INFO("Vulkan Layers are enabled, Validations-Layer-Debugger created.");
	}



}  // namespace venus
