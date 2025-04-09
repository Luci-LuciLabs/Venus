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

            // layer must have been found, return true.
            return true;
        };

        void setupDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger){
            if(!ENABLE_VALIDATION_LAYERS) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo = populateDebugMessengerCreateInfo();

            if(createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger)){
                throw std::runtime_error("FAILED TO SETUP DEBUG MESSENGER!!!");
            }
            
            std::cout << "DEBUG MESSENGER CREATED :3 " << '\n';
        }

        VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo(void){

            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

            createInfo.messageSeverity =   (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
            
            createInfo.messageType =   (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);
                            
            createInfo.pfnUserCallback = debugCallback;

            createInfo.pUserData = nullptr;

            return createInfo;
        }

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
            }

        
    } // namespace instanceLayers   
} // namespace venus_core
