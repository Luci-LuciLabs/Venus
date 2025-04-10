#ifndef VENUS_VULKAN_DEVICE_CONFIG_HPP
#define VENUS_VULKAN_DEVICE_CONFIG_HPP

#include "../volk/volk.h"

//stdlib
#include <vector>

namespace venus_core{

    inline static const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    
} // namespace venus_core

#endif // VENUS_VULKAN_DEVICE_CONFIG_HPP