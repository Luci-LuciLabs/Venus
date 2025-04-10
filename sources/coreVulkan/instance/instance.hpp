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

            VkInstance getInstance(void) const {return m_instance;}
        
        private:

            VkInstance m_instance = VK_NULL_HANDLE;
            VkDebugUtilsMessengerEXT m_debugMessenger;

    };
} // namespace venus_core


#endif // VENUS_VULKAN_INSTANCE_HPP