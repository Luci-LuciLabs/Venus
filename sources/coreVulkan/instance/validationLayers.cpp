#include "validationLayers.hpp"
#include "instanceConfig.hpp"

// stdlib
#include <vector>
#include <cstdint>
#include <iostream>
#include <cstring>

namespace venus_core{
    namespace instanceLayers{
      
        bool checkValidationLayerSupport(void){
            // DO NOT CHANGE initial population of layers
            uint32_t layerCount = 0;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            // we define which layers we want to use within VALIDATION_LAYERS inside of instanceConfig.hpp
            // we then can check for layer support, if changing layers then edit instanceConfig.hpp VALIDATION_LAYERS.
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

            // layer must have been found, return true.
            return true;
        };

        void setupDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger){
            if(!ENABLE_VALIDATION_LAYERS) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo = populateDebugMessengerCreateInfo();

            if(createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger)){
                throw std::runtime_error("FAILED TO SETUP DEBUG MESSENGER!!!");
            }
        
        }

        VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo(void){
            // entirety of debugMessenger createInfo can be edited here.
            // do not add any create info outside of this function.
            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

            createInfo.messageSeverity =   (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
            
            createInfo.messageType =   (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);
                            
            // DO NOT CHANGE!!!
            // debugCallback MUST be called here.
            // we do not use userData.
            createInfo.pfnUserCallback = debugCallback;
            createInfo.pUserData = nullptr;
            return createInfo;
        }

        // ==========================================================================================================
        // ALL FUNCTIONS BELOW THIS LINE SHOULD NOT BE CHANGED!!! THEY ARE INTEGRAL TO ALL VALIDATION LAYER OUTPUT!!!
        // ==========================================================================================================
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData){
                std::cerr << "Validation Layer: " << pCallbackData->pMessage << '\n';

                return VK_FALSE;
            }

        VkResult createDebugUtilsMessengerEXT(  VkInstance &instance,
                                                const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                const VkAllocationCallbacks *pAllocator,
                                                VkDebugUtilsMessengerEXT *pDebugMessenger)
            {
                auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT");

                if(func != nullptr){
                    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
                }else{
                    std::cerr << "Could not acquire vkCreateDebugUtilsMessengerEXT procAddr!" << '\n';
                    return VK_ERROR_EXTENSION_NOT_PRESENT;
                }
            }
        
        void destroyDebugUtilsMessengerEXT( VkInstance &instance,
                                            VkDebugUtilsMessengerEXT debugMessenger,
                                            const VkAllocationCallbacks *pAllocator)
            {
                auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT");

                if(func != nullptr){
                    func(instance, debugMessenger, pAllocator);
                }

                std::cout << "validation-messenger destroyed." << '\n';
            }

        
    } // namespace instanceLayers   
} // namespace venus_core
