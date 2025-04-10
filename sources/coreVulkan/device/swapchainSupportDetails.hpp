#ifndef VENUS_DEVICE_SWAPCHAIN_SUPPORT_HPP
#define VENUS_DEVICE_SWAPCHAIN_SUPPORT_HPP

#include "../volk/volk.h"

// stdlib
#include <vector>

namespace venus_core{

    // if any changes are made to SwapchainSupportDetails then you must also update querySwapchainSupportDetails()

    struct SwapchainSupportDetails{
        
        VkSurfaceCapabilitiesKHR surfaceCapabilites;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;

    }; // SwapchainSupportDetails
    
    SwapchainSupportDetails querySwapchainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    
} // namespace venus_core

#endif // VENUS_DEVICE_SWAPCHAIN_SUPPORT_HPP