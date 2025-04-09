#ifndef VENUS_VULKAN_INSTANCE_HPP
#define VENUS_VULKAN_INSTANCE_HPP

#include "../volk/volk.h"


namespace venus_core{
    class instance_core{
        public:
            instance_core();
            ~instance_core();

            instance_core(const instance_core &) = delete;
            instance_core &operator=(const instance_core &) = delete;
        
        private:

            VkInstance member_Instance = VK_NULL_HANDLE;
            VkDebugUtilsMessengerEXT debugMessenger;

    };
} // namespace venus_core


#endif // VENUS_VULKAN_INSTANCE_HPP