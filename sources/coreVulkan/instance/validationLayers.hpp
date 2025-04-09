#include "../volk/volk.h"


namespace venus_core{
    namespace instanceLayers{
    
        bool checkValidationLayerSupport(void);

        void setupDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger);

        VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo(void);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData);

        VkResult createDebugUtilsMessengerEXT(  VkInstance &instance,
                                                const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                const VkAllocationCallbacks *pAllocator,
                                                VkDebugUtilsMessengerEXT *pDebugMessenger);
        
        void destroyDebugUtilsMessengerEXT( VkInstance &instance,
                                            VkDebugUtilsMessengerEXT debugMessenger,
                                            const VkAllocationCallbacks *pAllocator);


    } // namespace instanceLayers  
} // namespace venus_core
