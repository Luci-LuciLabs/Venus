#include "glfwExtensions.hpp"
#include "instanceConfig.hpp"

#define VK_NO_PROTOTYPES
#include "../volk/volk.h"
#include <GLFW/glfw3.h>

// stdlib
#include <cstdint>


namespace venus_core{
    namespace instanceExtensions{
        
        std::vector<const char*> requiredInstanceExtensions(void){
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
            std::vector<const char*> instanceExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if(ENABLE_VALIDATION_LAYERS){
                instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return instanceExtensions;
        }

    } // namespace instanceExtensions   
} // namespace venus_core