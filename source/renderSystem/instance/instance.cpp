#include "instance.hpp"
// we initialize glfw here because the instance is required for everything else, including the window.
#include <GLFW/glfw3.h>

// STDLIB
#include <stdexcept>
#include <cstring>
#include <iostream>


namespace venus::rendersystem {

	instance::instance() {}
	instance::~instance() {}

  void instance::createInstance(void){
    if(glfwInit() != GLFW_TRUE) {
			throw std::runtime_error("Failed to initialize GLFW.");
		}
		if(volkInitialize() != VK_SUCCESS) {
			throw std::runtime_error("Failed to initialize Volk.");
		}
    if(ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()){
      throw std::runtime_error("Validation layers requested but not available.");
    }

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Venus application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		appInfo.pEngineName = "Venus";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_4;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

    auto instanceExtensions = getRequiredExtentions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    if(ENABLE_VALIDATION_LAYERS){
      createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
      createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }else{
      createInfo.enabledLayerCount = 0;
      createInfo.ppEnabledLayerNames = NULL;
    }

    if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS){
      throw std::runtime_error("Failed to create vulkan instance.");
    }

		volkLoadInstance(m_instance);
    setupDebugMessenger(m_instance, m_debugMessenger);
  }

  void instance::destroyInstance(void){

    if(ENABLE_VALIDATION_LAYERS){
      destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
    }
    m_debugMessenger = VK_NULL_HANDLE;

    if(m_instance != VK_NULL_HANDLE){
      vkDestroyInstance(m_instance, nullptr);
    }
    m_instance = VK_NULL_HANDLE;

		glfwTerminate();
		volkFinalize();
  }



	std::vector<const char *> instance::getRequiredExtentions(void) {
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if(ENABLE_VALIDATION_LAYERS) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool instance::checkValidationLayerSupport(void) {
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(const char* layerName : VALIDATION_LAYERS){
      bool layerFound = false;

      for(const auto& layerProperties : availableLayers){
        if(strcmp(layerName, layerProperties.layerName) == 0){
          layerFound = true;
          break;
        }
      }

      if(!layerFound){
        return false;
      }
    }

    return true;
  }

	void instance::setupDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger) {
    if(!ENABLE_VALIDATION_LAYERS){
      return;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT|
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT|
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT|
                                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

    debugCreateInfo.messageType = (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT|
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT|
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = nullptr;

    if(createDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS){
      throw std::runtime_error("Failed to create debug utils messenger.");
    }

  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // clang-format off
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																										 VkDebugUtilsMessageTypeFlagsEXT messageType,
																										 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
																										 void *pUserData) 
  {
      std::cerr << "Validation Layer: " << pCallbackData->pMessage << '\n';
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
        std::cerr << "Could not acquire vkCreateDebugUtilsMessengerEXT proc addr!\n"; 
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
  }
  
  // clang-format on
}  // namespace venus::rendersystem
