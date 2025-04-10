#ifndef VENUS_VULKAN_DEVICE_HPP
#define VENUS_VULKAN_DEVICE_HPP

#include "../volk/volk.h"
#include "../instance/instance.hpp"
#include "../window/glfwWindow.hpp"

//stdlib


namespace venus_core{
    class device_core{
        public:
            device_core( instance_core  &instance_coreRef, window_core &window_coreRef);
            ~device_core();

            // device_core should not be copied.
            device_core(const device_core &) = delete;
            device_core &operator=(const device_core &) = delete;
            // device_core should not be moved.
            device_core(const device_core &&) = delete;
            device_core &operator=(const device_core &&) = delete;
        
            VkPhysicalDevice getPhysicalDevice(void) const {return m_physicalDevice;}

            
        private:

            VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
            VkDevice m_logicalDevice = VK_NULL_HANDLE;

            VkQueue graphicsQueue = VK_NULL_HANDLE;
            VkQueue presentQueue = VK_NULL_HANDLE;

            instance_core &m_instance;
            window_core &m_window;

            // functions associated with physical device acquisition
            void pickPhysicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
            bool areRequiredExtensionsSupported(VkPhysicalDevice physicalDevice);
            int ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice);

            void createLogicalDevice(void);
    };
} // namespace venus_core


#endif // VENUS_VULKAN_DEVICE_HPP