#include "../volk/volk.h"

// These function are default-bare-minimum implementations of vulkan validation layer terminal logging.
// when NDEBUG is defined they are not used.
// when DEBUG is defined they are used to output vulkan warning/error reporting.

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
